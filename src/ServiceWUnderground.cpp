// Windy - ServiceWUnderground.cpp
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#include "ServiceWUnderground.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QXmlStreamReader>


ServiceWUnderground::ServiceWUnderground(QObject *parent) :
		WeatherService(parent) {

}

bool ServiceWUnderground::fetchCurrentConditions() {

	// this service requires API key
	if (getApiKey().isEmpty())
		return false;

	QUrl url("http://api.wunderground.com/api/" + getApiKey() + "/conditions/q/" + getLocation() + ".xml");
	QNetworkReply *reply = m_network_manager.get(QNetworkRequest(url));
	connect(reply, SIGNAL(readyRead()), SLOT(dispatchCurrentConditions()));

	qDebug() << "Request URL:" << url;

	return true;
}

void ServiceWUnderground::dispatchCurrentConditions() {

	// initialize condition structure with zeroed values
	WeatherConditions conditions = {};

	QXmlStreamReader xml(qobject_cast<QNetworkReply *>(sender()));

	while (!xml.atEnd())
		if (xml.readNextStartElement() && !xml.isEndElement()) {

			if (xml.name() == "station_id")
				conditions.stationID = xml.readElementText();
			else if (xml.name() == "observation_epoch")
				conditions.observationTime.setTime_t(xml.readElementText().toUInt());
			else if (xml.name() == "observation_location") {
				while (!(xml.isEndElement() and xml.name() == "observation_location"))
					if (xml.readNextStartElement() && !xml.isEndElement()) {
						if (xml.name() == "full")
							conditions.stationName = xml.readElementText();
						else if (xml.name() == "latitude")
							conditions.stationLatitude = xml.readElementText().toFloat();
						else if (xml.name() == "longitude")
							conditions.stationLongitude = xml.readElementText().toFloat();
						else if (xml.name() == "elevation")
							conditions.stationElevation = xml.readElementText().split(" ")[0].toFloat() / 3.2808;
					}
			}
			else if (xml.name() == "temp_c")
				conditions.temperature = xml.readElementText().toFloat() + 274.15;
			else if (xml.name() == "pressure_mb")
				conditions.pressure = xml.readElementText().toFloat() * 100;
			else if (xml.name() == "wind_kph")
				conditions.windSpeed = xml.readElementText().toFloat() * 10 / 36;
			else if (xml.name() == "wind_gust_kph")
				conditions.windGustSpeed = xml.readElementText().toFloat() * 10 / 36;
			else if (xml.name() == "wind_degrees")
				conditions.windDirection = xml.readElementText().toFloat();
			else if (xml.name() == "windchill_c" || xml.name() == "feelslike_c") {
				if (!conditions.windChill)
					conditions.windChill = xml.readElementText().toFloat() + 274.15;
			}
			else if (xml.name() == "relative_humidity")
				conditions.humidity = xml.readElementText().toFloat();
			else if (xml.name() == "dewpoint_c")
				conditions.dewPoint = xml.readElementText().toFloat() + 274.15;
			else if (xml.name() == "visibility_km")
				conditions.visibility = xml.readElementText().toFloat() * 1000;

		}

	dumpWeatherConditions(conditions);
	emit currentConditions(conditions);
}

bool ServiceWUnderground::fetchLocationAutocomplete(const QString &location) {

	QUrl url("http://autocomplete.wunderground.com/aq");
	url.addQueryItem("format", "xml");
	url.addQueryItem("query", location);

	QNetworkReply *reply = m_network_manager.get(QNetworkRequest(url));
	connect(reply, SIGNAL(readyRead()), SLOT(dispatchLocationAutocomplete()));

	qDebug() << "Request URL:" << url;

	return true;
}

void ServiceWUnderground::dispatchLocationAutocomplete() {

	QStringList names;
	QXmlStreamReader xml(qobject_cast<QNetworkReply *>(sender()));

	while (!xml.atEnd())
		if (xml.readNextStartElement() and !xml.isEndElement()) {
			if (xml.name() == "name")
				names << xml.readElementText();
		}

	emit locationAutocomplete(names);
}
