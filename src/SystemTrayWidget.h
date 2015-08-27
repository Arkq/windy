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


class SystemTrayWidget : public QObject {
	Q_OBJECT

public:
	explicit SystemTrayWidget(QObject *parent = 0);

	enum IconType {
		Clear,
		ClearNight,
		FewClouds,
		FewCloudsNight,
		Overcast,
		Showers,
		ShowersScattered,
		Fog,
		Snow,
		Storm,
		SevereAlert,
	};

	void setIcon(IconType type);

	// menu action accessors for the convenience's sake
	const QAction *getActionRefresh() const { return &m_action_refresh; }
	const QAction *getActionPreferences() const { return &m_action_preferences; }
	const QAction *getActionAbout() const { return &m_action_about; }
	const QAction *getActionQuit() const { return &m_action_quit; }

public slots:
	virtual void showIcon();

private:
	QSystemTrayIcon m_tray_icon;

	QMenu m_context_menu;
	QAction m_action_refresh;
	QAction m_action_preferences;
	QAction m_action_about;
	QAction m_action_quit;

};

#endif
