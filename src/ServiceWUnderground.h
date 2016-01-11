// Windy - ServiceWUnderground.h
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#ifndef WINDY_SERVICE_WUNDERGROUN_H_
#define WINDY_SERVICE_WUNDERGROUN_H_

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
