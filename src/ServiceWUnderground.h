// Windy - ServiceWUnderground.h
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#ifndef __SERVICE_WUNDERGROUN_H
#define __SERVICE_WUNDERGROUN_H

#include "WeatherService.h"

#include <QNetworkAccessManager>


class ServiceWUnderground : public WeatherService {
	Q_OBJECT

public:
	explicit ServiceWUnderground(QObject *parent = 0);

public slots:
	virtual bool fetchCurrentConditions();
	virtual bool fetchForecastConditions() { return false; }
	virtual bool fetchLocationAutocomplete(const QString &location);

private slots:
	void dispatchCurrentConditions();
	void dispatchLocationAutocomplete();

private:
	QNetworkAccessManager m_network_manager;

};

#endif
