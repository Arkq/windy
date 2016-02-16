// Windy - Settings.cpp
// Copyright (c) 2015-2016 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#include "Settings.h"

#include <QSettings>

#include "Application.h"


Settings::Settings(QObject *parent) :
		QObject(parent),
		m_data_update_interval(60),
		m_data_service(WeatherService::GoogleSearch),
		m_use_custom_command(false),
		m_unit_pressure(UnitPressure::Hectopascal),
		m_unit_temperature(UnitTemperature::Celsius),
		m_unit_wind_speed(UnitWindSpeed::KilometerPerHour),
		m_show_pressure(true),
		m_show_temperature(true),
		m_show_visibility(false),
		m_show_wind_speed(true),
		m_show_wind_chill(false),
		m_show_wind_gust_speed(false) {
}

void Settings::load() {

	QSettings settings("windy", "windy");

	settings.beginGroup("Appearance");
	m_show_pressure = settings.value("Pressure", m_show_pressure).toBool();
	m_show_temperature = settings.value("Temperature", m_show_temperature).toBool();
	m_show_visibility = settings.value("Visibility", m_show_visibility).toBool();
	m_show_wind_chill = settings.value("WindChill", m_show_wind_chill).toBool();
	m_show_wind_gust_speed = settings.value("WindGustSpeed", m_show_wind_gust_speed).toBool();
	m_show_wind_speed = settings.value("WindSpeed", m_show_wind_speed).toBool();
	settings.endGroup();

	settings.beginGroup("Units");
	m_unit_pressure = static_cast<UnitPressure>(settings.value("Pressure", static_cast<int>(m_unit_pressure)).toInt());
	m_unit_temperature = static_cast<UnitTemperature>(settings.value("Temperature", static_cast<int>(m_unit_temperature)).toInt());
	m_unit_wind_speed = static_cast<UnitWindSpeed>(settings.value("WindSpeed", static_cast<int>(m_unit_wind_speed)).toInt());
	settings.endGroup();

	settings.beginGroup("Service");
	m_data_update_interval = settings.value("UpdateInterval", m_data_update_interval).toInt();
	m_data_service = static_cast<WeatherService>(settings.value("Service", static_cast<int>(m_data_service)).toInt());
	m_wu_api_key = settings.value("WUndergroundApiKey").toString();
	m_wu_location = settings.value("WUndergroundLocation", m_wu_location).toString();
	m_yahoo_location = settings.value("YahooLocation", m_yahoo_location).toString();
	settings.endGroup();

	settings.beginGroup("Misc");
	m_use_custom_command = settings.value("UseCustomCmd", m_use_custom_command).toBool();
	m_custom_command = settings.value("CustomCmd", m_custom_command).toString();
	settings.endGroup();

}

void Settings::save() {

	QSettings settings("windy", "windy");

	settings.beginGroup("Appearance");
	settings.setValue("Pressure", m_show_pressure);
	settings.setValue("Temperature", m_show_temperature);
	settings.setValue("Visibility", m_show_visibility);
	settings.setValue("WindChill", m_show_wind_chill);
	settings.setValue("WindGustSpeed", m_show_wind_gust_speed);
	settings.setValue("WindSpeed", m_show_wind_speed);
	settings.endGroup();

	settings.beginGroup("Units");
	settings.setValue("Pressure", static_cast<int>(m_unit_pressure));
	settings.setValue("Temperature", static_cast<int>(m_unit_temperature));
	settings.setValue("WindSpeed", static_cast<int>(m_unit_wind_speed));
	settings.endGroup();

	settings.beginGroup("Service");
	settings.setValue("UpdateInterval", m_data_update_interval);
	settings.setValue("Service", static_cast<int>(m_data_service));
	settings.setValue("WUndergroundApiKey", m_wu_api_key);
	settings.setValue("WUndergroundLocation", m_wu_location);
	settings.setValue("YahooLocation", m_yahoo_location);
	settings.endGroup();

	settings.beginGroup("Misc");
	settings.setValue("UseCustomCmd", m_use_custom_command);
	settings.setValue("CustomCmd", m_custom_command);
	settings.endGroup();

}

Settings *Settings::settings() {
	return &Application::instance()->m_settings;
}
