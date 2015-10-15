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

	virtual QString getConditionsUrl() override;

public slots:
	virtual bool fetchCurrentConditions() override;
	virtual bool fetchForecastConditions() override { return false; }

protected:
	bool fetchLocationAutocomplete(const QString &query);

private slots:
	void dispatchCurrentConditions();
	void dispatchLocationAutocomplete();

private:
	QNetworkAccessManager m_network_manager;

	// station identifier saved from the lastly fetched conditions
	QString m_current_wmo;

};

#endif
