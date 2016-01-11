// Windy - PreferencesDialog.h
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#ifndef WINDY_PREFERENCES_DIALOG_H_
#define WINDY_PREFERENCES_DIALOG_H_

#include <QDialog>
#include <QScopedPointer>


namespace Ui {
	class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
	Q_OBJECT

public:
	PreferencesDialog(QWidget *parent = 0);
	~PreferencesDialog();

private slots:
	virtual void loadSettings();
	virtual void saveSettings();
	virtual void maintainServices();

private:
	QScopedPointer<Ui::PreferencesDialog> m_ui;

};

#endif
