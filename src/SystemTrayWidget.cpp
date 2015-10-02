// Windy - SystemTrayWidget.cpp
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#include "SystemTrayWidget.h"

#include "Settings.h"


SystemTrayWidget::SystemTrayWidget(QObject *parent) :
		QObject(parent),
		m_action_refresh(QIcon::fromTheme("view-refresh"), tr("&Refresh"), parent),
		m_action_preferences(QIcon::fromTheme("document-properties"), tr("&Preferences"), parent),
		m_action_about(QIcon::fromTheme("help-about"), tr("&About"), parent),
		m_action_quit(QIcon::fromTheme("application-exit"), tr("&Quit"), parent) {

	// assembly widget's context menu
	m_tray_icon.setContextMenu(&m_context_menu);
	m_context_menu.addAction(&m_action_refresh);
	m_context_menu.addAction(&m_action_preferences);
	m_context_menu.addAction(&m_action_about);
	m_context_menu.addSeparator();
	m_context_menu.addAction(&m_action_quit);

	connect(&m_action_refresh, SIGNAL(triggered()), SLOT(dispatchMenuAction()));
	connect(&m_action_preferences, SIGNAL(triggered()), SLOT(dispatchMenuAction()));
	connect(&m_action_about, SIGNAL(triggered()), SLOT(dispatchMenuAction()));
	connect(&m_action_quit, SIGNAL(triggered()), SLOT(dispatchMenuAction()));

	// initialize tray icon to the default value
	m_conditions.icon = WeatherConditions::WeatherIcon::Clear;
	updateIcon();

	m_tray_icon.show();

}

void SystemTrayWidget::setWeatherConditions(const WeatherConditions &conditions) {

	// update our internal weather conditions structure
	m_conditions = conditions;

	updateIcon();
	updateToolTip();

}

void SystemTrayWidget::updateIcon() {

	QStringList icons;

	switch (m_conditions.icon) {
	case WeatherConditions::WeatherIcon::Clear:
		icons << "weather-clear-symbolic";
		icons << "weather-clear";
		break;
	case WeatherConditions::WeatherIcon::ClearNight:
		icons << "weather-clear-night-symbolic";
		icons << "weather-night-clear";
		break;
	case WeatherConditions::WeatherIcon::FewClouds:
		icons << "weather-few-clouds-symbolic";
		icons << "weather-few-clouds";
		break;
	case WeatherConditions::WeatherIcon::FewCloudsNight:
		icons << "weather-few-clouds-night-symbolic";
		icons << "weather-few-clouds-night";
		break;
	case WeatherConditions::WeatherIcon::Overcast:
		icons << "weather-overcast-symbolic";
		icons << "weather-overcast";
		break;
	case WeatherConditions::WeatherIcon::Showers:
		icons << "weather-showers-symbolic";
		icons << "weather-showers";
		break;
	case WeatherConditions::WeatherIcon::ShowersScattered:
		icons << "weather-showers-scattered-symbolic";
		icons << "weather-showers-scattered";
		break;
	case WeatherConditions::WeatherIcon::Fog:
		icons << "weather-fog-symbolic";
		icons << "weather-fog";
		break;
	case WeatherConditions::WeatherIcon::Snow:
		icons << "weather-snow-symbolic";
		icons << "weather-snow";
		break;
	case WeatherConditions::WeatherIcon::Storm:
		icons << "weather-storm-symbolic";
		icons << "weather-storm";
		break;
	case WeatherConditions::WeatherIcon::SevereAlert:
		icons << "weather-severe-alert-symbolic";
		icons << "weather-severe-alert";
		break;
	}

	// iterate over icon candidates and set the first one which is available
	for (QStringList::ConstIterator it = icons.begin(); it != icons.end(); it++) {
		QIcon icon(QIcon::fromTheme(*it));
		if (!icon.isNull()) {
			m_tray_icon.setIcon(icon);
			return;
		}
	}

	qWarning() << "Weather status icon not found:" << icons;
}

void SystemTrayWidget::updateToolTip() {

	Settings *settings = Settings::settings();

	QString stationName(m_conditions.stationName);

	QString unitTemperature;
	QString unitPressure;
	QString unitDistance;
	QString unitSpeed;

	QString temperature(tr("n/a"));
	QString pressure(tr("n/a"));
	QString windSpeed(tr("n/a"));
	QString windGustSpeed(tr("n/a"));
	QString windDirection(tr("n/a"));
	QString windChill(tr("n/a"));
	QString humidity(tr("n/a"));
	QString dewPoint(tr("n/a"));
	QString visibility(tr("n/a"));

	if (m_conditions.windDirection != -1)
		windDirection.setNum(m_conditions.windDirection, 'f', 0);
	if (m_conditions.humidity != -1)
		humidity.setNum(m_conditions.humidity, 'f', 0);

	switch (settings->getUnitTemperature()) {
	case Settings::UnitTemperature::Celsius:
		unitTemperature = trUtf8("°C");
		if (m_conditions.temperature != -1)
			temperature.setNum(m_conditions.temperature - 273.15, 'f', 1);
		if (m_conditions.windChill != -1)
			windChill.setNum(m_conditions.windChill - 273.15, 'f', 1);
		if (m_conditions.dewPoint != -1)
			dewPoint.setNum(m_conditions.dewPoint - 273.15, 'f', 1);
		break;
	case Settings::UnitTemperature::Fahrenheit:
		unitTemperature = trUtf8("°F");
		if (m_conditions.temperature != -1)
			temperature.setNum((m_conditions.temperature - 273.15) * 1.8 + 32, 'f', 1);
		if (m_conditions.windChill != -1)
			windChill.setNum((m_conditions.windChill - 273.15) * 1.8 + 32, 'f', 1);
		if (m_conditions.dewPoint != -1)
			dewPoint.setNum((m_conditions.dewPoint - 273.15) * 1.8 + 32, 'f', 1);
		break;
	case Settings::UnitTemperature::Kelvin:
		unitTemperature = trUtf8("K");
		if (m_conditions.temperature != -1)
			temperature.setNum(m_conditions.temperature, 'f', 1);
		if (m_conditions.windChill != -1)
			windChill.setNum(m_conditions.windChill, 'f', 1);
		if (m_conditions.dewPoint != -1)
			dewPoint.setNum(m_conditions.dewPoint, 'f', 1);
		break;
	}

	switch (settings->getUnitPressure()) {
	case Settings::UnitPressure::Hectopascal:
		unitPressure = trUtf8("hPa");
		if (m_conditions.pressure != -1)
			pressure.setNum(m_conditions.pressure / 100, 'f', 0);
		break;
	case Settings::UnitPressure::PoundPerSquareInch:
		unitPressure = trUtf8("psi");
		if (m_conditions.pressure != -1)
			pressure.setNum(m_conditions.pressure / 6894.75729, 'f', 1);
		break;
	case Settings::UnitPressure::MillimeterOfMercury:
		unitPressure = trUtf8("mmHg");
		if (m_conditions.pressure != -1)
			pressure.setNum(m_conditions.pressure / 133.3224, 'f', 0);
		break;
	}

	switch (settings->getUnitWindSpeed()) {
	case Settings::UnitWindSpeed::KilometerPerHour:
		unitDistance = trUtf8("km");
		unitSpeed = trUtf8("km/h");
		if (m_conditions.windSpeed != -1)
			windSpeed.setNum(m_conditions.windSpeed * 3.6, 'f', 0);
		if (m_conditions.windGustSpeed != -1)
			windGustSpeed.setNum(m_conditions.windGustSpeed * 3.6, 'f', 0);
		if (m_conditions.visibility != -1)
			visibility.setNum(m_conditions.visibility / 1000, 'f', 1);
		break;
	case Settings::UnitWindSpeed::MilePerHour:
		unitDistance = trUtf8("mi.");
		unitSpeed = trUtf8("MpH");
		if (m_conditions.windSpeed != -1)
			windSpeed.setNum(m_conditions.windSpeed / 0.44704, 'f', 0);
		if (m_conditions.windGustSpeed != -1)
			windGustSpeed.setNum(m_conditions.windGustSpeed / 0.44704, 'f', 0);
		if (m_conditions.visibility != -1)
			visibility.setNum(m_conditions.visibility / 1609.3472, 'f', 1);
		break;
	case Settings::UnitWindSpeed::MeterPerSecond:
		unitDistance = trUtf8("m");
		unitSpeed = trUtf8("m/s");
		if (m_conditions.windSpeed != -1)
			windSpeed.setNum(m_conditions.windSpeed, 'f', 1);
		if (m_conditions.windGustSpeed != -1)
			windGustSpeed.setNum(m_conditions.windGustSpeed, 'f', 1);
		if (m_conditions.visibility != -1)
			visibility.setNum(m_conditions.visibility, 'f', 0);
		break;
	}

	// trim station name if it exceeds 30 characters (prevent wrapping)
	if (stationName.length() > 30)
		stationName = stationName.left(28) + "...";

	QString messageTemplate(trUtf8(
				"<b>{NAME}</b><br/>"
				"Temperature: {TEMP} ({CHILL}) {T}<br/>"
				"Wind: {WIND} ({GUST}) {S} {WDIR}°<br/>"
				"Visibility: {VISIB} {D}<br/>"
				"Pressure: {PRES} {P}"));

	m_tray_icon.setToolTip(messageTemplate
			.replace("{T}", unitTemperature)
			.replace("{P}", unitPressure)
			.replace("{D}", unitDistance)
			.replace("{S}", unitSpeed)
			.replace("{NAME}", stationName)
			.replace("{TEMP}", temperature)
			.replace("{PRES}", pressure)
			.replace("{WIND}", windSpeed)
			.replace("{GUST}", windGustSpeed)
			.replace("{WDIR}", windDirection)
			.replace("{CHILL}", windChill)
			.replace("{HUMID}", humidity)
			.replace("{DEWPT}", dewPoint)
			.replace("{VISIB}", visibility));

}

void SystemTrayWidget::dispatchMenuAction() {

	QAction *action = qobject_cast<QAction *>(sender());

	if (action == &m_action_refresh)
		emit menuActionTriggered(MenuAction::Refresh);
	else if (action == &m_action_preferences)
		emit menuActionTriggered(MenuAction::Preferences);
	else if (action == &m_action_about)
		emit menuActionTriggered(MenuAction::About);
	else if (action == &m_action_quit)
		emit menuActionTriggered(MenuAction::Quit);

}
