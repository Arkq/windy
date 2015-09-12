// Windy - Application.cpp
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#include "Application.h"

#include "PreferencesDialog.h"
#include "ServiceYahooWeather.h"
#include "ServiceWUnderground.h"


Application::Application(int &argc, char **argv) :
		QApplication(argc, argv),
		m_tray_widget(&m_settings),
		m_service(nullptr),
		m_timer_id(0) {

	// this is a window-less application, hence exiting it upon closing
	// a dialog (e.g. preferences) is not a desirable behavior
	setQuitOnLastWindowClosed(false);

	m_settings.load();

	// setup weather service engine according to the user preferences
	setupWeatherService();
	setupUpdateTimer();

	// handle menu events from the tray icon widget
	connect(&m_tray_widget, SIGNAL(menuActionTriggered(SystemTrayWidget::MenuAction)),
			this, SLOT(dispatchMenuAction(SystemTrayWidget::MenuAction)));

	m_tray_widget.updateIcon(WeatherConditions::WeatherIcon::Clear);
	m_tray_widget.showIcon();

	// update data upon application start
	updateWeatherConditions();

}

void Application::updateWeatherConditions() {
	if (m_service)
		m_service->fetchCurrentConditions();
}

void Application::showPreferencesDialog() {
	PreferencesDialog *preferencesDialog = new PreferencesDialog(&m_settings);
	connect(preferencesDialog, SIGNAL(accepted()), SLOT(saveSettings()));
	preferencesDialog->show();
}

void Application::setupWeatherService() {

	// remove previously initialized service handler
	delete m_service;

	switch (m_settings.getDataService()) {
	case Settings::WeatherService::YahooWeather:
		m_service = new ServiceYahooWeather(this);
		break;
	case Settings::WeatherService::WeatherUnderground:
		m_service = new ServiceWUnderground(this);
		m_service->setApiKey(m_settings.getWUndergroundApiKey());
		m_service->setLocation(m_settings.getWUndergroundLocation());
		break;
	default:
		break;
	}

	if (m_service) {
		// data update is done asynchronously (GUI responsiveness is a primary
		// objective), so we need to listen for the update signal
		connect(m_service, SIGNAL(currentConditions(const WeatherConditions &)),
				&m_tray_widget, SLOT(updateConditions(const WeatherConditions &)));
	}

}

void Application::setupUpdateTimer() {

	if (m_timer_id)
		killTimer(m_timer_id);

	// one can disable automatic updates if it is an inconvenience
	if (m_settings.getDataUpdateInterval() > 0)
		m_timer_id = startTimer(m_settings.getDataUpdateInterval() * 60000);

}

void Application::timerEvent(QTimerEvent *event) {
	if (event->timerId() == m_timer_id)
		updateWeatherConditions();
}

void Application::dispatchMenuAction(SystemTrayWidget::MenuAction action) {
	switch (action) {
	case SystemTrayWidget::MenuAction::Refresh:
		updateWeatherConditions();
		// reinitialize timer event
		setupUpdateTimer();
		break;
	case SystemTrayWidget::MenuAction::Preferences:
		showPreferencesDialog();
		break;
	case SystemTrayWidget::MenuAction::About:
		aboutQt();
		break;
	case SystemTrayWidget::MenuAction::Quit:
		quit();
		break;
	}
}

void Application::saveSettings() {
	setupWeatherService();
	setupUpdateTimer();
	m_settings.save();
}
