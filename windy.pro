# Windy - windy.pro
# Copyright (c) 2015 Arkadiusz Bokowy

TEMPLATE = app

TARGET = windy
VERSION = 0.1.0

CONFIG += qt
CONFIG += c++11

QT += core gui
QT += widgets

unix {
	QMAKE_CXXFLAGS += -std=c++11
}

HEADERS += \
	src/Application.h \
	src/PreferencesDialog.h \
	src/Settings.h \
	src/SystemTrayWidget.h

SOURCES += \
	src/Application.cpp \
	src/PreferencesDialog.cpp \
	src/Settings.cpp \
	src/SystemTrayWidget.cpp \
	src/main.cpp

FORMS += \
	src/PreferencesDialog.ui
