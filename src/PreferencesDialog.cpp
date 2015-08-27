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

	connect(m_ui->buttonBox, SIGNAL(accepted()), SLOT(saveSettings()));
	connect(m_ui->buttonBox, SIGNAL(rejected()), SLOT(close()));

}

PreferencesDialog::~PreferencesDialog() {
}

void PreferencesDialog::loadSettings() {

	m_ui->spinBoxUpdateInterval->setValue(m_settings->getUpdateInterval());

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

	m_settings->setUpdateInterval(m_ui->spinBoxUpdateInterval->value());

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

	m_settings->save();

	close();
}
