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
		m_update_interval(60),
		m_unit_pressure(UnitPressure::Hectopascal),
		m_unit_temperature(UnitTemperature::Celsius),
		m_unit_wind(UnitWind::KilometerPerHour) {
}

void Settings::load() {

	QSettings settings("windy", "windy");

	m_update_interval = settings.value("UpdateInterval", m_update_interval).toInt();

	settings.beginGroup("Units");
	m_unit_pressure = static_cast<UnitPressure>(settings.value("Pressure", m_unit_pressure).toInt());
	m_unit_temperature = static_cast<UnitTemperature>(settings.value("Temperature", m_unit_temperature).toInt());
	m_unit_wind = static_cast<UnitWind>(settings.value("Wind", m_unit_wind).toInt());
	settings.endGroup();

}

void Settings::save() {

	QSettings settings("windy", "windy");

	settings.setValue("UpdateInterval", m_update_interval);

	settings.beginGroup("Units");
	settings.setValue("Pressure", m_unit_pressure);
	settings.setValue("Temperature", m_unit_temperature);
	settings.setValue("Wind", m_unit_wind);
	settings.endGroup();

}
