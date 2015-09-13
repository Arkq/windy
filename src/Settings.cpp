// Windy - Settings.cpp
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#include "Settings.h"

#include <QSettings>


Settings::Settings(QObject *parent) :
		QObject(parent),
		m_data_update_interval(60),
		m_unit_pressure(UnitPressure::Hectopascal),
		m_unit_temperature(UnitTemperature::Celsius),
		m_unit_wind_speed(UnitWindSpeed::KilometerPerHour) {
}

void Settings::load() {

	QSettings settings("windy", "windy");

	settings.beginGroup("Service");
	m_data_update_interval = settings.value("UpdateInterval", m_data_update_interval).toInt();
	m_data_service = static_cast<WeatherService>(settings.value("Service", static_cast<int>(m_data_service)).toInt());
	m_yahoo_api_key = settings.value("YahooApiKey").toString();
	m_wu_api_key = settings.value("WUndergroundApiKey").toString();
	m_wu_location = settings.value("WUndergroundLocation", m_wu_location).toString();
	settings.endGroup();

	settings.beginGroup("Units");
	m_unit_pressure = static_cast<UnitPressure>(settings.value("Pressure", static_cast<int>(m_unit_pressure)).toInt());
	m_unit_temperature = static_cast<UnitTemperature>(settings.value("Temperature", static_cast<int>(m_unit_temperature)).toInt());
	m_unit_wind_speed = static_cast<UnitWindSpeed>(settings.value("WindSpeed", static_cast<int>(m_unit_wind_speed)).toInt());
	settings.endGroup();

}

void Settings::save() {

	QSettings settings("windy", "windy");

	settings.beginGroup("Service");
	settings.setValue("UpdateInterval", m_data_update_interval);
	settings.setValue("Service", static_cast<int>(m_data_service));
	settings.setValue("YahooApiKey", m_yahoo_api_key);
	settings.setValue("WUndergroundApiKey", m_wu_api_key);
	settings.setValue("WUndergroundLocation", m_wu_location);
	settings.endGroup();

	settings.beginGroup("Units");
	settings.setValue("Pressure", static_cast<int>(m_unit_pressure));
	settings.setValue("Temperature", static_cast<int>(m_unit_temperature));
	settings.setValue("WindSpeed", static_cast<int>(m_unit_wind_speed));
	settings.endGroup();

}
