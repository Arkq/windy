// Windy - ServiceWUnderground.cpp
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This project is licensed under the terms of the MIT license.

#include "ServiceWUnderground.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QXmlStreamReader>
#if QT_VERSION >= 0x050000
	#include <QUrlQuery>
#endif


ServiceWUnderground::ServiceWUnderground(QObject *parent) :
		WeatherService(parent) {
}

QString ServiceWUnderground::getConditionsUrl() {
	if (m_current_wmo.isEmpty())
		return QString();
	return "http://www.wunderground.com/global/stations/" + m_current_wmo + ".html";
}

bool ServiceWUnderground::fetchCurrentConditions() {

	// this service requires API key
	if (getApiKey().isEmpty())
		return false;

	QString location(getLocation());

	// fall-back to the automatic detection if location is not provided
	if (location.isEmpty())
		location = "autoip";

	QUrl url("http://api.wunderground.com/api/" + getApiKey() + "/conditions/q/" + location + ".xml");

	qDebug() << "Current conditions:" << url;
	QNetworkReply *reply = m_network_manager.get(QNetworkRequest(url));
	connect(reply, SIGNAL(finished()), SLOT(dispatchCurrentConditions()));

	return true;
}

void ServiceWUnderground::dispatchCurrentConditions() {

	WeatherConditions conditions = getEmptyWeatherConditions();

	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
	QXmlStreamReader xml(reply);

	while (!xml.atEnd())
		if (xml.readNextStartElement() && !xml.isEndElement()) {

			if (xml.name() == "station_id")
				conditions.stationID = xml.readElementText();
			else if (xml.name() == "observation_epoch")
				conditions.observationTime.setTime_t(xml.readElementText().toUInt());
			else if (xml.name() == "display_location") {
				while (!(xml.isEndElement() && xml.name() == "display_location"))
					if (xml.readNextStartElement() && !xml.isEndElement()) {
						if (xml.name() == "full")
							conditions.locationName = xml.readElementText();
						else if (xml.name() == "wmo")
							m_current_wmo = xml.readElementText();
						else if (xml.name() == "latitude")
							conditions.locationLatitude = xml.readElementText().toFloat();
						else if (xml.name() == "longitude")
							conditions.locationLongitude = xml.readElementText().toFloat();
						else if (xml.name() == "elevation")
							conditions.locationElevation = xml.readElementText().remove("ft").toFloat() / 3.2808;
					}
			}
			else if (xml.name() == "temp_c")
				conditions.temperature = xml.readElementText().toFloat() + 273.15;
			else if (xml.name() == "pressure_mb")
				conditions.pressure = xml.readElementText().toFloat() * 100;
			else if (xml.name() == "wind_kph")
				conditions.windSpeed = xml.readElementText().toFloat() * 10 / 36;
			else if (xml.name() == "wind_gust_kph")
				conditions.windGustSpeed = xml.readElementText().toFloat() * 10 / 36;
			else if (xml.name() == "wind_degrees")
				conditions.windDirection = 360 - xml.readElementText().toFloat();
			else if (xml.name() == "windchill_c" || xml.name() == "feelslike_c") {
				QString tmp(xml.readElementText());
				if (tmp != "NA" && conditions.windChill == -1)
					conditions.windChill = tmp.toFloat() + 273.15;
			}
			else if (xml.name() == "relative_humidity")
				conditions.humidity = xml.readElementText().remove("%").toFloat();
			else if (xml.name() == "dewpoint_c")
				conditions.dewPoint = xml.readElementText().toFloat() + 273.15;
			else if (xml.name() == "visibility_km") {
				QString tmp(xml.readElementText());
				if (tmp != "N/A")
					conditions.visibility = tmp.toFloat() * 1000;
			}
			else if (xml.name() == "icon_url") {

				QString icon = xml.readElementText().section('/', -1).section('.', 0, 0);
				bool nighttime = icon.startsWith("nt_");
				icon = icon.mid(3);

				if (icon == "clear" || icon == "sunny")
					conditions.icon = nighttime ? WeatherConditions::WeatherIcon::ClearNight : WeatherConditions::WeatherIcon::Clear;
				else if (icon == "partlycloudy" || icon == "mostlysunny" || icon == "chanceflurries")
					conditions.icon = nighttime ? WeatherConditions::WeatherIcon::FewCloudsNight : WeatherConditions::WeatherIcon::FewClouds;
				else if (icon == "cloudy" || icon == "mostlycloudy" || icon == "partlysunny" || icon == "chancesnow")
					conditions.icon = WeatherConditions::WeatherIcon::Overcast;
				else if (icon == "rain")
					conditions.icon = WeatherConditions::WeatherIcon::Showers;
				else if (icon == "chancerain" || icon == "chancesleet")
					conditions.icon = WeatherConditions::WeatherIcon::ShowersScattered;
				else if (icon == "fog" || icon == "hazy")
					conditions.icon = WeatherConditions::WeatherIcon::Fog;
				else if (icon == "snow" || icon == "flurries" || icon == "sleet")
					conditions.icon = WeatherConditions::WeatherIcon::Snow;
				else if (icon == "tstorms" || icon == "chancetstorms")
					conditions.icon = WeatherConditions::WeatherIcon::Storm;

			}

		}

	emit currentConditions(conditions);
	reply->deleteLater();
}

bool ServiceWUnderground::fetchLocationAutocomplete(const QString &query) {

	// do not bother if the query string is empty
	if (query.isEmpty())
		return false;

	QUrl url("http://autocomplete.wunderground.com/aq");
#if QT_VERSION >= 0x050000
	QUrlQuery urlQuery(url);
	urlQuery.addQueryItem("format", "xml");
	urlQuery.addQueryItem("query", query);
	url.setQuery(urlQuery);
#else
	url.addQueryItem("format", "xml");
	url.addQueryItem("query", query);
#endif

	qDebug() << "Autocomplete:" << url;
	QNetworkReply *reply = m_network_manager.get(QNetworkRequest(url));
	connect(reply, SIGNAL(finished()), SLOT(dispatchLocationAutocomplete()));

	return true;
}

void ServiceWUnderground::dispatchLocationAutocomplete() {

	QStringList names;
	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
	QXmlStreamReader xml(reply);

	while (!xml.atEnd())
		if (xml.readNextStartElement() && !xml.isEndElement()) {
			if (xml.name() == "name")
				names << xml.readElementText();
		}

//	emit locationAutocomplete(names);
	reply->deleteLater();
}
