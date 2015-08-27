// Windy - SystemTrayWidget.cpp
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#include "SystemTrayWidget.h"

#include <QDebug>


SystemTrayWidget::SystemTrayWidget(QObject *parent) :
		QObject(parent),
		m_action_refresh(QIcon::fromTheme("view-refresh"), "&Refresh", parent),
		m_action_preferences(QIcon::fromTheme("document-properties"), "&Preferences", parent),
		m_action_about(QIcon::fromTheme("help-about"), "&About", parent),
		m_action_quit(QIcon::fromTheme("application-exit"), "&Quit", parent) {

	// initialize context menu - add actions
	m_tray_icon.setContextMenu(&m_context_menu);
	m_context_menu.addAction(&m_action_refresh);
	m_context_menu.addAction(&m_action_preferences);
	m_context_menu.addAction(&m_action_about);
	m_context_menu.addSeparator();
	m_context_menu.addAction(&m_action_quit);

}

void SystemTrayWidget::setIcon(IconType type) {

	QStringList icons;
	QIcon icon;

	switch (type) {
	case Clear:
		icons << "weather-clear-symbolic"
			<< "weather-clear";
		break;
	case ClearNight:
		icons << "weather-clear-night-symbolic"
			<< "weather-night-clear";
		break;
	case FewClouds:
		icons << "weather-few-clouds-symbolic"
			<< "weather-few-clouds";
		break;
	case FewCloudsNight:
		icons << "weather-few-clouds-night-symbolic"
			<< "weather-few-clouds-night";
		break;
	case Overcast:
		icons << "weather-overcast-symbolic"
			<< "weather-overcast";
		break;
	case Showers:
		icons << "weather-showers-symbolic"
			<< "weather-showers";
		break;
	case ShowersScattered:
		icons << "weather-showers-scattered-symbolic"
			<< "weather-showers-scattered";
		break;
	case Fog:
		icons << "weather-fog-symbolic"
			<< "weather-fog";
		break;
	case Snow:
		icons << "weather-snow-symbolic"
			<< "weather-snow";
		break;
	case Storm:
		icons << "weather-storm-symbolic"
			<< "weather-storm";
		break;
	case SevereAlert:
		icons << "weather-severe-alert-symbolic"
			<< "weather-severe-alert";
		break;
	}

	// iterate over icon candidates and set the first one which is available
	for (QStringList::ConstIterator it = icons.begin(); it != icons.end(); it++) {
		icon = QIcon::fromTheme(*it);
		if (!icon.isNull()) {
			m_tray_icon.setIcon(icon);
			return;
		}
	}

	qWarning() << "Weather status icon not found: " << icons;
}

void SystemTrayWidget::showIcon() {
	m_tray_icon.show();
}
