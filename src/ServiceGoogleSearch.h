// Windy - ServiceGoogleSearch.h
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#ifndef WINDY_SERVICE_GOOGLE_SEARCH_H_
#define WINDY_SERVICE_GOOGLE_SEARCH_H_

#include "WeatherService.h"

#include <QNetworkAccessManager>


class ServiceGoogleSearch : public WeatherService {
	Q_OBJECT

public:
	explicit ServiceGoogleSearch(QObject *parent = 0);

	virtual QString getConditionsUrl() override;

public slots:
	virtual bool fetchCurrentConditions() override;
	virtual bool fetchForecastConditions() override { return false; }

private slots:
	void dispatchCurrentConditions();

protected:
	QNetworkRequest buildNetworkRequest(const QUrl &url);

private:
	QNetworkAccessManager m_network_manager;

};

#endif
