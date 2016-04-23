// Windy - ServiceYahooWeather.h
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This project is licensed under the terms of the MIT license.

#ifndef WINDY_SERVICE_YAHOO_WEATHER_H_
#define WINDY_SERVICE_YAHOO_WEATHER_H_

#include "WeatherService.h"

#include <QNetworkAccessManager>


class ServiceYahooWeather : public WeatherService {
	Q_OBJECT

public:
	explicit ServiceYahooWeather(QObject *parent = 0);

public slots:
	virtual bool fetchCurrentConditions() override;
	virtual bool fetchForecastConditions() override { return false; }

private slots:
	void dispatchCurrentConditions();

protected:
	int getWoeid(const QString &location);

private:
	QNetworkAccessManager m_network_manager;

	// previously resolved Where On Earth Identifiers
	QHash<QString, int> m_woeid_cache;

};

#endif
