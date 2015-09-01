// Windy - SystemTrayWidget.cpp
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#include "SystemTrayWidget.h"


SystemTrayWidget::SystemTrayWidget(QObject *parent) :
		QObject(parent),
		m_action_refresh(QIcon::fromTheme("view-refresh"), "&Refresh", parent),
		m_action_preferences(QIcon::fromTheme("document-properties"), "&Preferences", parent),
		m_action_about(QIcon::fromTheme("help-about"), "&About", parent),
		m_action_quit(QIcon::fromTheme("application-exit"), "&Quit", parent) {

	// assembly widget's context menu
	m_tray_icon.setContextMenu(&m_context_menu);
	m_context_menu.addAction(&m_action_refresh);
	m_context_menu.addAction(&m_action_preferences);
	m_context_menu.addAction(&m_action_about);
	m_context_menu.addSeparator();
	m_context_menu.addAction(&m_action_quit);

	connect(&m_action_refresh, SIGNAL(triggered()), SLOT(dispatchMenuAction()));
	connect(&m_action_preferences, SIGNAL(triggered()), SLOT(dispatchMenuAction()));
	connect(&m_action_about, SIGNAL(triggered()), SLOT(dispatchMenuAction()));
	connect(&m_action_quit, SIGNAL(triggered()), SLOT(dispatchMenuAction()));

}

void SystemTrayWidget::setIcon(IconType type) {

	QStringList icons;
	QIcon icon;

	switch (type) {
	case IconType::Clear:
		icons << "weather-clear-symbolic";
		icons << "weather-clear";
		break;
	case IconType::ClearNight:
		icons << "weather-clear-night-symbolic";
		icons << "weather-night-clear";
		break;
	case IconType::FewClouds:
		icons << "weather-few-clouds-symbolic";
		icons << "weather-few-clouds";
		break;
	case IconType::FewCloudsNight:
		icons << "weather-few-clouds-night-symbolic";
		icons << "weather-few-clouds-night";
		break;
	case IconType::Overcast:
		icons << "weather-overcast-symbolic";
		icons << "weather-overcast";
		break;
	case IconType::Showers:
		icons << "weather-showers-symbolic";
		icons << "weather-showers";
		break;
	case IconType::ShowersScattered:
		icons << "weather-showers-scattered-symbolic";
		icons << "weather-showers-scattered";
		break;
	case IconType::Fog:
		icons << "weather-fog-symbolic";
		icons << "weather-fog";
		break;
	case IconType::Snow:
		icons << "weather-snow-symbolic";
		icons << "weather-snow";
		break;
	case IconType::Storm:
		icons << "weather-storm-symbolic";
		icons << "weather-storm";
		break;
	case IconType::SevereAlert:
		icons << "weather-severe-alert-symbolic";
		icons << "weather-severe-alert";
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

	qWarning() << "Weather status icon not found:" << icons;
}

void SystemTrayWidget::showIcon(bool show) {
	m_tray_icon.setVisible(show);
}

void SystemTrayWidget::updateConditions(const WeatherConditions &conditions) {
	m_tray_icon.setToolTip("<b>LOL</b><br/>Wiadomosc!");
}

void SystemTrayWidget::dispatchMenuAction() {

	QAction *action = qobject_cast<QAction *>(sender());

	if (action == &m_action_refresh)
		emit menuActionTriggered(MenuAction::Refresh);
	else if (action == &m_action_preferences)
		emit menuActionTriggered(MenuAction::Preferences);
	else if (action == &m_action_about)
		emit menuActionTriggered(MenuAction::About);
	else if (action == &m_action_quit)
		emit menuActionTriggered(MenuAction::Quit);

}
