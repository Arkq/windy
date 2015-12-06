// Windy - ServiceYahooWeather.cpp
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#include "ServiceYahooWeather.h"

#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QXmlStreamReader>


ServiceYahooWeather::ServiceYahooWeather(QObject *parent) :
		WeatherService(parent) {
}

bool ServiceYahooWeather::fetchCurrentConditions() {

	// the weather feed does not support IP-based location
	if (getLocation().isEmpty())
		return false;

	QString woeid(getLocation());
	if (!QRegExp("\\d+").exactMatch(woeid))
		woeid.setNum(getWoeid(woeid));

	if (woeid.isEmpty())
		return false;

	QUrl url("http://weather.yahooapis.com/forecastrss?u=c&w=" + woeid);

	qDebug() << "Current conditions:" << url;
	QNetworkReply *reply = m_network_manager.get(QNetworkRequest(url));
	connect(reply, SIGNAL(finished()), SLOT(dispatchCurrentConditions()));

	return true;
}

void ServiceYahooWeather::dispatchCurrentConditions() {

	// initialize condition structure with "zeroed" values
	WeatherConditions conditions = {
		"", "", 0, 0, 0,
		QDateTime::currentDateTime(),
		WeatherConditions::WeatherIcon::Clear,
		-1, -1, -1, -1, -1, -1, -1, -1, -1
	};

	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
	QXmlStreamReader xml(reply);

	while (!xml.atEnd())
		if (xml.readNextStartElement() && !xml.isEndElement()) {

			if (xml.name() == "location") {
				QStringList location({
						xml.attributes().value("city").toString(),
						xml.attributes().value("region").toString(),
						xml.attributes().value("country").toString()});
				conditions.locationName = location.filter(QRegExp("\\S+")).join(", ");
			}
			else if (xml.name() == "lat")
				conditions.locationLatitude = xml.readElementText().toFloat();
			else if (xml.name() == "long")
				conditions.locationLongitude = xml.readElementText().toFloat();
			else if (xml.name() == "condition") {
				conditions.temperature = xml.attributes().value("temp").toString().toFloat() + 273.15;
				switch (xml.attributes().value("code").toString().toInt()) {
#if 0
				case 23:  // blustery
				case 24:  // windy
				case 25:  // cold
				case 32:  // sunny
				case 34:  // fair (day)
				case 36:  // hot
					conditions.icon = WeatherConditions::WeatherIcon::Clear;
					break;
#endif
				case 31:  // clear (night)
				case 33:  // fair (night)
					conditions.icon = WeatherConditions::WeatherIcon::ClearNight;
					break;
				case 30:  // partly cloudy (day)
				case 44:  // partly cloudy
					conditions.icon = WeatherConditions::WeatherIcon::FewClouds;
					break;
				case 29:  // partly cloudy (night)
					conditions.icon = WeatherConditions::WeatherIcon::FewCloudsNight;
					break;
				case 26:  // cloudy
				case 27:  // mostly cloudy (night)
				case 28:  // mostly cloudy (day)
					conditions.icon = WeatherConditions::WeatherIcon::Overcast;
					break;
				case 11:  // showers
				case 12:  // showers
				case 17:  // hail
				case 35:  // mixed rain and hail
					conditions.icon = WeatherConditions::WeatherIcon::Showers;
					break;
				case 9:   // drizzle
				case 40:  // scattered showers
					conditions.icon = WeatherConditions::WeatherIcon::ShowersScattered;
					break;
				case 19:  // dust
				case 20:  // foggy
				case 21:  // haze
				case 22:  // smoky
					conditions.icon = WeatherConditions::WeatherIcon::Fog;
					break;
				case 5:   // mixed rain and snow
				case 6:   // mixed rain and sleet
				case 7:   // mixed snow and sleet
				case 8:   // freezing drizzle
				case 10:  // freezing rain
				case 13:  // snow flurries
				case 14:  // light snow showers
				case 15:  // blowing snow
				case 16:  // snow
				case 18:  // sleet
				case 41:  // heavy snow
				case 42:  // scattered snow showers
				case 43:  // heavy snow
				case 46:  // snow showers
					conditions.icon = WeatherConditions::WeatherIcon::Snow;
					break;
				case 4:   // thunderstorms
				case 37:  // isolated thunderstorms
				case 38:  // scattered thunderstorms
				case 39:  // scattered thunderstorms
				case 45:  // thundershowers
				case 47:  // isolated thundershowers
					conditions.icon = WeatherConditions::WeatherIcon::Storm;
					break;
				case 0:   // tornado
				case 1:   // tropical storm
				case 2:   // hurricane
				case 3:   // severe thunderstorms
					conditions.icon = WeatherConditions::WeatherIcon::SevereAlert;
					break;
				}
			}
			else if (xml.name() == "wind") {
				conditions.windChill = xml.attributes().value("chill").toString().toFloat() + 273.15;
				conditions.windSpeed = xml.attributes().value("speed").toString().toFloat() * 10 / 36;
				conditions.windDirection = 360 - xml.attributes().value("direction").toString().toInt();
			}
			else if (xml.name() == "atmosphere") {
				conditions.humidity = xml.attributes().value("humidity").toString().toFloat();
				conditions.visibility = xml.attributes().value("visibility").toString().toFloat() * 1000;
				conditions.pressure = xml.attributes().value("pressure").toString().toFloat() * 100;
			}

		}

	emit currentConditions(conditions);
	reply->deleteLater();
}

int ServiceYahooWeather::getWoeid(const QString &location) {

	// if given location was previously resolved, return WOEID from cache
	if (m_woeid_cache.contains(location))
		return m_woeid_cache[location];

	QString query("select woeid from geo.places(1) where text = \"" + location + "\"");
	QUrl url("http://query.yahooapis.com/v1/public/yql?format=xml&q=" + query);

	qDebug() << "WOEID:" << url;
	QNetworkReply *reply = m_network_manager.get(QNetworkRequest(url));

	// NOTE: Unfortunately, the Network Manager class doesn't support synchronous
	//       calls (which is reasonable, because it will hog GUI responsiveness).
	//       However, in our case, the simplicity of a synchronous call over a
	//       signaling nightmare is a way to go - the response is cached anyway.
	QEventLoop loop;
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	int woeid = 0;

	// we're interested in only one field in the returned XML response, so
	// a simple regular expression matching will suffice for this purpose
	QRegExp rx("<woeid>(\\d+)</woeid>");
	if (rx.indexIn(reply->readAll()) != -1)
		m_woeid_cache[location] = woeid = rx.cap(1).toInt();

	delete reply;
	return woeid;
}
