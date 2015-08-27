// Windy - Application.h
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#ifndef __APPLICATION_H
#define __APPLICATION_H

#include <QApplication>

#include "Settings.h"
#include "SystemTrayWidget.h"


class Application : public QApplication {
	Q_OBJECT

public:
	Application(int &argc, char **argv);

public slots:
	virtual void showPreferencesDialog();

private:
	Settings m_settings;
	SystemTrayWidget m_tray_widget;

};

#endif
