// Windy - Application.h
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#ifndef __APPLICATION_H
#define __APPLICATION_H

#include <QApplication>

#include "Settings.h"
#include "SystemTrayWidget.h"
#include "WeatherService.h"


class Application : public QApplication {
	Q_OBJECT

public:
	Application(int &argc, char **argv);

public slots:
	void updateWeatherConditions();
	void showPreferencesDialog();

protected:
	void setupWeatherService();
	void setupUpdateTimer();
	void timerEvent(QTimerEvent *event);

private slots:
	void dispatchMenuAction(SystemTrayWidget::MenuAction action);
	void saveSettings();

private:
	Settings m_settings;
	SystemTrayWidget m_tray_widget;
	WeatherService *m_service;
	int m_timer_id;

};

#endif
