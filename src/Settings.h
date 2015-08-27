// Windy - Settings.h
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#ifndef __SETTINGS_H
#define __SETTINGS_H

#include <QMetaType>
#include <QObject>


class Settings : public QObject {
	Q_OBJECT

public:
	explicit Settings(QObject *parent = 0);

	enum UnitPressure {
		Hectopascal,
		PoundPerSquareInch,
		MillimeterOfMercury,
	};

	enum UnitTemperature {
		Celsius,
		Fahrenheit,
		Kelvin,
	};

	enum UnitWind {
		KilometerPerHour,
		MilePerHour,
		MeterPerSecond,
	};

	int getUpdateInterval() const { return m_update_interval; }
	void setUpdateInterval(int value) { m_update_interval = value; }

	UnitPressure getUnitPressure() const { return m_unit_pressure; }
	void setUnitPressure(UnitPressure value) { m_unit_pressure = value; }
	UnitTemperature getUnitTemperature() const { return m_unit_temperature; }
	void setUnitTemperature(UnitTemperature value) { m_unit_temperature = value; }
	UnitWind getUnitWind() const { return m_unit_wind; }
	void setUnitWind(UnitWind value) { m_unit_wind = value; }

	void load();
	void save();

private:
	int m_update_interval;

	UnitPressure m_unit_pressure;
	UnitTemperature m_unit_temperature;
	UnitWind m_unit_wind;

};

Q_DECLARE_METATYPE(Settings::UnitPressure)
Q_DECLARE_METATYPE(Settings::UnitTemperature)
Q_DECLARE_METATYPE(Settings::UnitWind)

#endif
