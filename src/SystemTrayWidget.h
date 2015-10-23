// Windy - SystemTrayWidget.h
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#ifndef __SYSTEM_TRAY_H
#define __SYSTEM_TRAY_H

#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>

#include "WeatherService.h"


class SystemTrayWidget : public QObject {
	Q_OBJECT

public:
	explicit SystemTrayWidget(QObject *parent = 0);

	enum class MenuAction {
		Refresh,
		Preferences,
		About,
		Quit,
	};

public slots:
	void setWeatherConditions(const WeatherConditions &conditions);
	void updateIcon();
	void updateToolTip();

private slots:
	void dispatchIconActivation(QSystemTrayIcon::ActivationReason reason);
	void dispatchMenuAction();

protected:
	QString getLabelNotAvailable() const { return tr("n/a"); }
	QString getUnitDistance() const;
	QString getUnitPressure() const;
	QString getUnitSpeed() const;
	QString getUnitTemperature() const;

signals:
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void menuActionTriggered(SystemTrayWidget::MenuAction action);

private:

	QSystemTrayIcon m_tray_icon;
	QMenu m_context_menu;
	QAction m_action_refresh;
	QAction m_action_preferences;
	QAction m_action_about;
	QAction m_action_quit;

	// currently displayed weather conditions
	WeatherConditions m_conditions;

};

#endif
