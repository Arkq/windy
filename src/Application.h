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

// homepage URL used in the About dialog
#define WINDY_HOMEPAGE "https://github.com/Arkq/windy"


class Application : public QApplication {
	Q_OBJECT

	// global settings object is provided by the Settings class
	friend class Settings;

public:
	Application(int &argc, char **argv);

	// provide convenient access to our global application handler
	static Application *instance() {
		return static_cast<Application *>(QApplication::instance());
	}

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
