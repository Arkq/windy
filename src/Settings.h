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

	enum class WeatherService {
		Undefined = 0,
		YahooWeather,
		WeatherUnderground,
	};

	enum class UnitPressure {
		Hectopascal,
		PoundPerSquareInch,
		MillimeterOfMercury,
	};

	enum class UnitTemperature {
		Celsius,
		Fahrenheit,
		Kelvin,
	};

	enum class UnitWindSpeed {
		KilometerPerHour,
		MilePerHour,
		MeterPerSecond,
	};

	int getDataUpdateInterval() const { return m_data_update_interval; }
	void setDataUpdateInterval(int value) { m_data_update_interval = value; }
	WeatherService getDataService() const { return m_data_service; }
	void setDataService(WeatherService value) { m_data_service = value; }

	const QString &getYahooApiKey() const { return m_yahoo_api_key; }
	void setYahooApiKey(const QString &value) { m_yahoo_api_key = value; }
	const QString &getWUndergroundApiKey() const { return m_wu_api_key; }
	void setWUndergroundApiKey(const QString &value) { m_wu_api_key = value; }
	const QString &getWUndergroundLocation() const { return m_wu_location; }
	void setWUndergroundLocation(const QString &value) { m_wu_location = value; }

	UnitPressure getUnitPressure() const { return m_unit_pressure; }
	void setUnitPressure(UnitPressure value) { m_unit_pressure = value; }
	UnitTemperature getUnitTemperature() const { return m_unit_temperature; }
	void setUnitTemperature(UnitTemperature value) { m_unit_temperature = value; }
	UnitWindSpeed getUnitWindSpeed() const { return m_unit_wind_speed; }
	void setUnitWindSpeed(UnitWindSpeed value) { m_unit_wind_speed = value; }

	void load();
	void save();

	// provide access to the global application settings
	static Settings *settings();

private:

	int m_data_update_interval;
	WeatherService m_data_service;

	QString m_yahoo_api_key;
	QString m_wu_api_key;
	QString m_wu_location;

	UnitPressure m_unit_pressure;
	UnitTemperature m_unit_temperature;
	UnitWindSpeed m_unit_wind_speed;

};

Q_DECLARE_METATYPE(Settings::WeatherService)
Q_DECLARE_METATYPE(Settings::UnitPressure)
Q_DECLARE_METATYPE(Settings::UnitTemperature)
Q_DECLARE_METATYPE(Settings::UnitWindSpeed)

#endif
