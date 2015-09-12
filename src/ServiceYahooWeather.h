// Windy - ServiceYahooWeather.h
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#ifndef __SERVICE_YAHOO_WEATHER_H
#define __SERVICE_YAHOO_WEATHER_H

#include "WeatherService.h"


class ServiceYahooWeather : public WeatherService {
	Q_OBJECT

public:
	explicit ServiceYahooWeather(QObject *parent = 0);

public slots:
	virtual bool fetchCurrentConditions() { return false; }
	virtual bool fetchForecastConditions() { return false; }
	virtual bool fetchLocationAutocomplete(const QString &location) { return false; }

};

#endif
