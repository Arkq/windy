// Windy - Application.cpp
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#include "Application.h"

#include "PreferencesDialog.h"


Application::Application(int &argc, char **argv) :
		QApplication(argc, argv) {

	// this is a window-less application, hence exiting it upon closing
	// a dialog (e.g. preferences) is not a desirable behavior
	setQuitOnLastWindowClosed(false);

	m_settings.load();

	connect(m_tray_widget.getActionPreferences(), SIGNAL(triggered()), SLOT(showPreferencesDialog()));
	connect(m_tray_widget.getActionAbout(), SIGNAL(triggered()), SLOT(aboutQt()));
	connect(m_tray_widget.getActionQuit(), SIGNAL(triggered()), SLOT(quit()));

	m_tray_widget.setIcon(SystemTrayWidget::IconType::Clear);
	m_tray_widget.showIcon();

}

void Application::showPreferencesDialog() {
	PreferencesDialog *preferencesDialog = new PreferencesDialog(&m_settings);
	preferencesDialog->show();
}
