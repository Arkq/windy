// Windy - Settings.h
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#ifndef WINDY_SETTINGS_H_
#define WINDY_SETTINGS_H_

#include <QMetaType>
#include <QObject>


class Settings : public QObject {
	Q_OBJECT

public:
	explicit Settings(QObject *parent = 0);

	enum class WeatherService {
		Undefined = 0,
		GoogleSearch,
		WeatherUnderground,
		YahooWeather,
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

	const QString &getWUndergroundApiKey() const { return m_wu_api_key; }
	void setWUndergroundApiKey(const QString &value) { m_wu_api_key = value; }
	const QString &getWUndergroundLocation() const { return m_wu_location; }
	void setWUndergroundLocation(const QString &value) { m_wu_location = value; }
	const QString &getYahooLocation() const { return m_yahoo_location; }
	void setYahooLocation(const QString &value) { m_yahoo_location = value; }

	UnitPressure getUnitPressure() const { return m_unit_pressure; }
	void setUnitPressure(UnitPressure value) { m_unit_pressure = value; }
	UnitTemperature getUnitTemperature() const { return m_unit_temperature; }
	void setUnitTemperature(UnitTemperature value) { m_unit_temperature = value; }
	UnitWindSpeed getUnitWindSpeed() const { return m_unit_wind_speed; }
	void setUnitWindSpeed(UnitWindSpeed value) { m_unit_wind_speed = value; }

	bool getShowPressure() const { return m_show_pressure; }
	void setShowPressure(bool value) { m_show_pressure = value; }
	bool getShowTemperature() const { return m_show_temperature; }
	void setShowTemperature(bool value) { m_show_temperature = value; }
	bool getShowVisibility() const { return m_show_visibility; }
	void setShowVisibility(bool value) { m_show_visibility = value; }
	bool getShowWindChill() const { return m_show_wind_chill; }
	void setShowWindChill(bool value) { m_show_wind_chill = value; }
	bool getShowWindGustSpeed() const { return m_show_wind_gust_speed; }
	void setShowWindGustSpeed(bool value) { m_show_wind_gust_speed = value; }
	bool getShowWindSpeed() const { return m_show_wind_speed; }
	void setShowWindSpeed(bool value) { m_show_wind_speed = value; }

	void load();
	void save();

	// provide access to the global application settings
	static Settings *settings();

private:

	int m_data_update_interval;
	WeatherService m_data_service;

	QString m_wu_api_key;
	QString m_wu_location;
	QString m_yahoo_location;

	UnitPressure m_unit_pressure;
	UnitTemperature m_unit_temperature;
	UnitWindSpeed m_unit_wind_speed;

	bool m_show_pressure;
	bool m_show_temperature;
	bool m_show_visibility;
	bool m_show_wind_speed;
	bool m_show_wind_chill;
	bool m_show_wind_gust_speed;

};

Q_DECLARE_METATYPE(Settings::WeatherService)
Q_DECLARE_METATYPE(Settings::UnitPressure)
Q_DECLARE_METATYPE(Settings::UnitTemperature)
Q_DECLARE_METATYPE(Settings::UnitWindSpeed)

#endif
