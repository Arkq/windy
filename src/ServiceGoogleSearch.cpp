// Windy - ServiceGoogleSearch.cpp
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#include "ServiceGoogleSearch.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QXmlStreamReader>


ServiceGoogleSearch::ServiceGoogleSearch(QObject *parent) :
		WeatherService(parent) {
}

QString ServiceGoogleSearch::getConditionsUrl() {
	return "http://www.google.com/search?q=weather";
}

bool ServiceGoogleSearch::fetchCurrentConditions() {

	QUrl url("http://www.google.com/search?q=weather");

	qDebug() << "Current conditions:" << url;
	QNetworkReply *reply = m_network_manager.get(buildNetworkRequest(url));
	connect(reply, SIGNAL(finished()), SLOT(dispatchCurrentConditions()));

	return true;
}

void ServiceGoogleSearch::dispatchCurrentConditions() {

	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
	QUrl redirectUrl(reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl());

	// Since Google is a bot-net, it wants to track everyone and everywhere.
	// Simple *.com query is immediately redirected to the local domain based
	// on the client IP address. Also there is some tricky game with tracking
	// tokens and cookies - redirecting without a cookie-jar won't work!.
	if (!redirectUrl.isEmpty()) {
		qDebug() << "Redirection:" << redirectUrl;
		QNetworkReply *reply2 = m_network_manager.get(buildNetworkRequest(redirectUrl));
		connect(reply2, SIGNAL(finished()), SLOT(dispatchCurrentConditions()));
	}
	else {

		// initialize condition structure with "zeroed" values
		WeatherConditions conditions = {
			"", "", 0, 0, 0,
			QDateTime::currentDateTime(),
			WeatherConditions::WeatherIcon::Clear,
			-1, -1, -1, -1, -1, -1, -1, -1, -1
		};

		QString html(QString::fromUtf8(reply->readAll()));
		QStringRef weatherHtml;

		// XXX: Result page seems to be heavily optimized (or messy written) for
		//      modern, forgiving HTML parsers. If page is correctly displayed,
		//      there is no need to bother about XML structure correctness, YOLO!
		//      Qt XML parser is very strict in that aspect, so it is impossible
		//      to parse modern HTML5 document..., but if we modify it a little
		//      bit, it might be possible to parse its small (localized) part.

		// rewind HTML content to the interesting stuff - weather conditions
		weatherHtml = html.midRef(html.indexOf("id=\"wob_wc\""));
		weatherHtml = html.midRef(weatherHtml.indexOf('<') + weatherHtml.position());

		// XML reader does not like ampersand characters in tag attributes,
		// so we need to strip them all beforehand. This action might cost
		// us a lot if location name contains HTML entities...
		html = weatherHtml.toString().left(5000).remove('&');

		// Remove every image tag from the document except the one which contains
		// weather condition icon. Image tag might not be correctly closed, which
		// will break our XML reader.
		html.remove(QRegExp("<img((?!wob_tci)[^>])*>"));

		QXmlStreamReader xml("<root><div>" + html);
		while (!xml.atEnd())
			if (xml.readNextStartElement()) {

				QStringRef attrId(xml.attributes().value("id"));

				if (attrId == "wob_loc")
					conditions.locationName = xml.readElementText();
				else if (attrId == "wob_tm")
					conditions.temperature = xml.readElementText().toFloat() + 273.15;
				else if (attrId == "wob_hm")
					conditions.humidity = xml.readElementText().remove("%").toFloat();
				else if (attrId == "wob_ws")
					conditions.windSpeed = xml.readElementText().remove("km/h").toFloat() * 10 / 36;
				else if (attrId == "wob_tci") {

					QString icon = xml.attributes().value("src").toString().section('/', -1).section('.', 0, 0);

					if (icon == "sunny" || icon == "sunny_s_cloudy")
						conditions.icon = WeatherConditions::WeatherIcon::Clear;
					else if (icon == "partly_cloudy" || icon == "cloudy_s_sunny")
						conditions.icon = WeatherConditions::WeatherIcon::FewClouds;
					else if (icon == "cloudy")
						conditions.icon = WeatherConditions::WeatherIcon::Overcast;
					else if (icon == "rain" || icon == "rain_heavy" || icon == "rain_s_cloudy" || icon == "cloudy_s_rain")
						conditions.icon = WeatherConditions::WeatherIcon::Showers;
					else if (icon == "rain_light" || icon == "sunny_s_rain")
						conditions.icon = WeatherConditions::WeatherIcon::ShowersScattered;
					else if (icon == "fog")
						conditions.icon = WeatherConditions::WeatherIcon::Fog;
					else if (icon == "show" || icon == "show_light" || icon == "show_s_rain")
						conditions.icon = WeatherConditions::WeatherIcon::Snow;
					else if (icon == "thunderstorms")
						conditions.icon = WeatherConditions::WeatherIcon::Storm;

				}

			}

		emit currentConditions(conditions);
	}

	reply->deleteLater();
}

QNetworkRequest ServiceGoogleSearch::buildNetworkRequest(const QUrl &url) {
	QNetworkRequest request(url);
	// Google handles tons of devices and browser versions, for each serving
	// different type of search results. From our point of view, the most usable
	// one, is the one which contains ID attributes in the weather section.
	request.setRawHeader("User-Agent", "Mozilla/5.0 (Linux; Android 4.3) AppleWebKit/536.23");
	return request;
}
