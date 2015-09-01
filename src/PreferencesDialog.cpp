// Windy - PreferencesDialog.cpp
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"


PreferencesDialog::PreferencesDialog(Settings *settings, QWidget *parent) :
		QDialog(parent),
		m_ui(new Ui::PreferencesDialog()),
		m_settings(settings) {

	m_ui->setupUi(this);

	loadSettings();

	// weather services are mutually exclusive, these connections will ensure,
	// that this paradigm is fulfilled by our GUI widgets
	connect(m_ui->groupYahoo, SIGNAL(toggled(bool)), SLOT(maintainServices()));
	connect(m_ui->groupWUnd, SIGNAL(toggled(bool)), SLOT(maintainServices()));

	connect(m_ui->buttonBox, SIGNAL(accepted()), SLOT(saveSettings()));
	connect(m_ui->buttonBox, SIGNAL(rejected()), SLOT(close()));

}

PreferencesDialog::~PreferencesDialog() {
}

void PreferencesDialog::loadSettings() {

	m_ui->spinBoxUpdateInterval->setValue(m_settings->getDataUpdateInterval());

	// enable selected service
	maintainServices();

	m_ui->lineEditWUndKey->setText(m_settings->getWUndergroundApiKey());
	m_ui->lineEditWUndLocation->setText(m_settings->getWUndergroundLocation());

	switch (m_settings->getUnitPressure()) {
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

	switch (m_settings->getUnitTemperature()) {
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

	switch (m_settings->getUnitWind()) {
	case Settings::UnitWind::KilometerPerHour:
		m_ui->radioButtonWindKMH->setChecked(true);
		break;
	case Settings::UnitWind::MilePerHour:
		m_ui->radioButtonWindMPH->setChecked(true);
		break;
	case Settings::UnitWind::MeterPerSecond:
		m_ui->radioButtonWindMS->setChecked(true);
		break;
	}

}

void PreferencesDialog::saveSettings() {

	m_settings->setDataUpdateInterval(m_ui->spinBoxUpdateInterval->value());

	if (m_ui->groupYahoo->isChecked())
		m_settings->setDataService(Settings::WeatherService::YahooWeather);
	else if (m_ui->groupWUnd->isChecked())
		m_settings->setDataService(Settings::WeatherService::WeatherUnderground);
	else
		m_settings->setDataService(Settings::WeatherService::Undefined);

	m_settings->setWUndergroundApiKey(m_ui->lineEditWUndKey->text());
	m_settings->setWUndergroundLocation(m_ui->lineEditWUndLocation->text());

	if (m_ui->radioButtonPressHPA->isChecked())
		m_settings->setUnitPressure(Settings::UnitPressure::Hectopascal);
	else if (m_ui->radioButtonPressPSI->isChecked())
		m_settings->setUnitPressure(Settings::UnitPressure::PoundPerSquareInch);
	else if (m_ui->radioButtonPressMMHG->isChecked())
		m_settings->setUnitPressure(Settings::UnitPressure::MillimeterOfMercury);

	if (m_ui->radioButtonTempC->isChecked())
		m_settings->setUnitTemperature(Settings::UnitTemperature::Celsius);
	else if (m_ui->radioButtonTempF->isChecked())
		m_settings->setUnitTemperature(Settings::UnitTemperature::Fahrenheit);
	else if (m_ui->radioButtonTempK->isChecked())
		m_settings->setUnitTemperature(Settings::UnitTemperature::Kelvin);

	if (m_ui->radioButtonWindKMH->isChecked())
		m_settings->setUnitWind(Settings::UnitWind::KilometerPerHour);
	else if (m_ui->radioButtonWindMPH->isChecked())
		m_settings->setUnitWind(Settings::UnitWind::MilePerHour);
	else if (m_ui->radioButtonWindMS->isChecked())
		m_settings->setUnitWind(Settings::UnitWind::MeterPerSecond);

	emit accepted();
	close();

}

void PreferencesDialog::maintainServices() {

	QGroupBox *group = qobject_cast<QGroupBox *>(sender());

	if (group == nullptr) {
		switch (m_settings->getDataService()) {
		case Settings::WeatherService::Undefined:
			m_ui->groupYahoo->setChecked(false);
			m_ui->groupWUnd->setChecked(false);
			break;
		case Settings::WeatherService::YahooWeather:
			m_ui->groupYahoo->setChecked(true);
			m_ui->groupWUnd->setChecked(false);
			break;
		case Settings::WeatherService::WeatherUnderground:
			m_ui->groupYahoo->setChecked(false);
			m_ui->groupWUnd->setChecked(true);
			break;
		}
	}
	else if (group == m_ui->groupYahoo && group->isChecked()) {
		m_ui->groupWUnd->setChecked(false);
	}
	else if (group == m_ui->groupWUnd && group->isChecked()) {
		m_ui->groupYahoo->setChecked(false);
	}

}
