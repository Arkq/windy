// Windy - WeatherService.h
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#ifndef __WEATHER_SERVICE_H
#define __WEATHER_SERVICE_H

#include <QDateTime>
#include <QDebug>
#include <QObject>
#include <QStringList>


struct WeatherConditions {

	enum class WeatherIcon {
		Clear,
		ClearNight,
		FewClouds,
		FewCloudsNight,
		Overcast,
		Showers,
		ShowersScattered,
		Fog,
		Snow,
		Storm,
		SevereAlert,
	};

	QString stationID;
	QString stationName;
	float stationLatitude;
	float stationLongitude;
	float stationElevation;

	QDateTime observationTime;

	WeatherIcon icon;

	// NOTE: If any of these attributes is set to -1 (invalid value from the
	// physical point of view), it will mean, that the data is not available.

	float temperature;   // Kelvin
	float pressure;      // Pascal
	float windSpeed;     // Meter per second
	float windGustSpeed; // Meter per second
	float windDirection; // Degrees (counterclockwise starting from north)
	float windChill;     // Kelvin
	float humidity;      // Percent
	float dewPoint;      // Kelvin
	float visibility;    // Meter

};

class WeatherService : public QObject {
	Q_OBJECT

public:
	WeatherService(QObject *parent = 0) : QObject(parent) {}

	void setApiKey(const QString &key) { m_api_key = key; }
	const QString &getApiKey() const { return m_api_key; }

	void setLocation(const QString &location) { m_location = location; }
	const QString &getLocation() const { return m_location; }

public slots:
	virtual bool fetchCurrentConditions() = 0;
	virtual bool fetchForecastConditions() = 0;
	virtual bool fetchLocationAutocomplete(const QString &location) = 0;

signals:
	void currentConditions(const WeatherConditions &conditions);
	void forecastConditions(const WeatherConditions &conditions);
	void locationAutocomplete(const QStringList &locations);

private:
	QString m_api_key;
	QString m_location;

};

// Dump weather condition structure data to the console. Note, that
// this function is for the debugging purposes only.
inline void dumpWeatherConditions(const WeatherConditions &conditions) {
	qDebug() << "station ID:" << conditions.stationID;
	qDebug() << "station name:" << conditions.stationName;
	qDebug() << "latitude:" << conditions.stationLatitude;
	qDebug() << "longitude:" << conditions.stationLongitude;
	qDebug() << "elevation:" << conditions.stationElevation << "m";
	qDebug() << "time:" << conditions.observationTime.toString();
	qDebug() << "icon:" << static_cast<int>(conditions.icon);
	qDebug() << "temperature:" << conditions.temperature << "K";
	qDebug() << "pressure:" << conditions.pressure << "Pa";
	qDebug() << "wind:" << conditions.windSpeed << "m/s";
	qDebug() << "wind gust:" << conditions.windGustSpeed << "m/s";
	qDebug() << "wind direction:" << conditions.windDirection << "deg";
	qDebug() << "wind chill:" << conditions.windChill << "K";
	qDebug() << "humidity:" << conditions.humidity << "%";
	qDebug() << "dew point:" << conditions.dewPoint << "K";
	qDebug() << "visibility" << conditions.visibility << "m";
}

#endif
