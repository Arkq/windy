// Windy - PreferencesDialog.h
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#ifndef __PREFERENCES_DIALOG_H
#define __PREFERENCES_DIALOG_H

#include <QDialog>
#include <QScopedPointer>

#include "Settings.h"


namespace Ui {
	class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
	Q_OBJECT

public:
	PreferencesDialog(Settings *settings, QWidget *parent = 0);
	~PreferencesDialog();

private slots:
	virtual void loadSettings();
	virtual void saveSettings();
	virtual void maintainServices();

private:
	QScopedPointer<Ui::PreferencesDialog> m_ui;
	Settings *m_settings;

};

#endif
