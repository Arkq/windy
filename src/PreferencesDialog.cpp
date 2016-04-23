// Windy - PreferencesDialog.cpp
// Copyright (c) 2015-2016 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This project is licensed under the terms of the MIT license.

#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

#include "Settings.h"


PreferencesDialog::PreferencesDialog(QWidget *parent) :
		QDialog(parent),
		m_ui(new Ui::PreferencesDialog()) {

	m_ui->setupUi(this);

	loadSettings();

	// weather services are mutually exclusive, these connections will ensure,
	// that this paradigm is fulfilled by our GUI widgets
	connect(m_ui->groupGoogle, SIGNAL(toggled(bool)), SLOT(maintainServices()));
	connect(m_ui->groupWUnd, SIGNAL(toggled(bool)), SLOT(maintainServices()));
	connect(m_ui->groupYahoo, SIGNAL(toggled(bool)), SLOT(maintainServices()));

	connect(m_ui->buttonBox, SIGNAL(accepted()), SLOT(saveSettings()));
	connect(m_ui->buttonBox, SIGNAL(rejected()), SLOT(close()));

}

PreferencesDialog::~PreferencesDialog() {
}

void PreferencesDialog::loadSettings() {

	Settings *settings = Settings::settings();

	m_ui->spinBoxUpdateInterval->setValue(settings->getDataUpdateInterval());

	m_ui->groupCommand->setChecked(settings->getUseCustomCommand());
	m_ui->lineEditCommand->setText(settings->getCustomCommand());

	// enable selected service
	maintainServices();

	m_ui->lineEditWUndKey->setText(settings->getWUndergroundApiKey());
	m_ui->lineEditWUndLocation->setText(settings->getWUndergroundLocation());
	m_ui->lineEditYahooLocation->setText(settings->getYahooLocation());

	m_ui->checkBoxPress->setChecked(settings->getShowPressure());
	m_ui->checkBoxTemp->setChecked(settings->getShowTemperature());
	m_ui->checkBoxVisib->setChecked(settings->getShowVisibility());
	m_ui->checkBoxWind->setChecked(settings->getShowWindSpeed());
	m_ui->checkBoxWindChill->setChecked(settings->getShowWindChill());
	m_ui->checkBoxWindGust->setChecked(settings->getShowWindGustSpeed());

	switch (settings->getUnitPressure()) {
	case Settings::UnitPressure::Hectopascal:
		m_ui->radioButtonPressHPA->setChecked(true);
		break;
	case Settings::UnitPressure::PoundPerSquareInch:
		m_ui->radioButtonPressPSI->setChecked(true);
		break;
	case Settings::UnitPressure::MillimeterOfMercury:
		m_ui->radioButtonPressMMHG->setChecked(true);
		break;
	}

	switch (settings->getUnitTemperature()) {
	case Settings::UnitTemperature::Celsius:
		m_ui->radioButtonTempC->setChecked(true);
		break;
	case Settings::UnitTemperature::Fahrenheit:
		m_ui->radioButtonTempF->setChecked(true);
		break;
	case Settings::UnitTemperature::Kelvin:
		m_ui->radioButtonTempK->setChecked(true);
		break;
	}

	switch (settings->getUnitWindSpeed()) {
	case Settings::UnitWindSpeed::KilometerPerHour:
		m_ui->radioButtonWindKMH->setChecked(true);
		break;
	case Settings::UnitWindSpeed::MilePerHour:
		m_ui->radioButtonWindMPH->setChecked(true);
		break;
	case Settings::UnitWindSpeed::MeterPerSecond:
		m_ui->radioButtonWindMS->setChecked(true);
		break;
	}

}

void PreferencesDialog::saveSettings() {

	Settings *settings = Settings::settings();

	settings->setDataUpdateInterval(m_ui->spinBoxUpdateInterval->value());

	settings->setUseCustomCommand(m_ui->groupCommand->isChecked());
	settings->setCustomCommand(m_ui->lineEditCommand->text());

	if (m_ui->groupGoogle->isChecked())
		settings->setDataService(Settings::WeatherService::GoogleSearch);
	else if (m_ui->groupWUnd->isChecked())
		settings->setDataService(Settings::WeatherService::WeatherUnderground);
	else if (m_ui->groupYahoo->isChecked())
		settings->setDataService(Settings::WeatherService::YahooWeather);
	else
		settings->setDataService(Settings::WeatherService::Undefined);

	settings->setWUndergroundApiKey(m_ui->lineEditWUndKey->text());
	settings->setWUndergroundLocation(m_ui->lineEditWUndLocation->text());
	settings->setYahooLocation(m_ui->lineEditYahooLocation->text());

	settings->setShowPressure(m_ui->checkBoxPress->isChecked());
	settings->setShowTemperature(m_ui->checkBoxTemp->isChecked());
	settings->setShowVisibility(m_ui->checkBoxVisib->isChecked());
	settings->setShowWindSpeed(m_ui->checkBoxWind->isChecked());
	settings->setShowWindChill(m_ui->checkBoxWindChill->isChecked());
	settings->setShowWindGustSpeed(m_ui->checkBoxWindGust->isChecked());

	if (m_ui->radioButtonPressHPA->isChecked())
		settings->setUnitPressure(Settings::UnitPressure::Hectopascal);
	else if (m_ui->radioButtonPressPSI->isChecked())
		settings->setUnitPressure(Settings::UnitPressure::PoundPerSquareInch);
	else if (m_ui->radioButtonPressMMHG->isChecked())
		settings->setUnitPressure(Settings::UnitPressure::MillimeterOfMercury);

	if (m_ui->radioButtonTempC->isChecked())
		settings->setUnitTemperature(Settings::UnitTemperature::Celsius);
	else if (m_ui->radioButtonTempF->isChecked())
		settings->setUnitTemperature(Settings::UnitTemperature::Fahrenheit);
	else if (m_ui->radioButtonTempK->isChecked())
		settings->setUnitTemperature(Settings::UnitTemperature::Kelvin);

	if (m_ui->radioButtonWindKMH->isChecked())
		settings->setUnitWindSpeed(Settings::UnitWindSpeed::KilometerPerHour);
	else if (m_ui->radioButtonWindMPH->isChecked())
		settings->setUnitWindSpeed(Settings::UnitWindSpeed::MilePerHour);
	else if (m_ui->radioButtonWindMS->isChecked())
		settings->setUnitWindSpeed(Settings::UnitWindSpeed::MeterPerSecond);

	emit accepted();
	close();

}

void PreferencesDialog::maintainServices() {

	QGroupBox *group = qobject_cast<QGroupBox *>(sender());

	if (group == nullptr) {
		switch (Settings::settings()->getDataService()) {
		case Settings::WeatherService::Undefined:
			m_ui->groupGoogle->setChecked(false);
			m_ui->groupWUnd->setChecked(false);
			m_ui->groupYahoo->setChecked(false);
			break;
		case Settings::WeatherService::GoogleSearch:
			m_ui->groupGoogle->setChecked(true);
			m_ui->groupWUnd->setChecked(false);
			m_ui->groupYahoo->setChecked(false);
			break;
		case Settings::WeatherService::WeatherUnderground:
			m_ui->groupGoogle->setChecked(false);
			m_ui->groupWUnd->setChecked(true);
			m_ui->groupYahoo->setChecked(false);
			break;
		case Settings::WeatherService::YahooWeather:
			m_ui->groupGoogle->setChecked(false);
			m_ui->groupWUnd->setChecked(false);
			m_ui->groupYahoo->setChecked(true);
			break;
		}
	}
	else if (group == m_ui->groupGoogle && group->isChecked()) {
		m_ui->groupWUnd->setChecked(false);
		m_ui->groupYahoo->setChecked(false);
	}
	else if (group == m_ui->groupWUnd && group->isChecked()) {
		m_ui->groupGoogle->setChecked(false);
		m_ui->groupYahoo->setChecked(false);
	}
	else if (group == m_ui->groupYahoo && group->isChecked()) {
		m_ui->groupGoogle->setChecked(false);
		m_ui->groupWUnd->setChecked(false);
	}

}
