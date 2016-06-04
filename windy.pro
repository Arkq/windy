# Windy - windy.pro
# Copyright (c) 2015-2016 Arkadiusz Bokowy

TEMPLATE = app

TARGET = windy
VERSION = 0.1.0

CONFIG += qt
CONFIG += c++11

QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
}

unix:!greaterThan(QT_MAJOR_VERSION, 4) {
	QMAKE_CXXFLAGS += -std=c++11
}

!unix {
	# Themed icons are supported on Unix platform only. On other
	# platforms bundled weather icons are mandatory.
	DEFINES += WITH_WEATHER_ICONS
}

HEADERS += \
	src/Application.h \
	src/PreferencesDialog.h \
	src/ServiceGoogleSearch.h \
	src/ServiceWUnderground.h \
	src/ServiceYahooWeather.h \
	src/Settings.h \
	src/SystemTrayWidget.h \
	src/WeatherService.h

SOURCES += \
	src/Application.cpp \
	src/PreferencesDialog.cpp \
	src/ServiceGoogleSearch.cpp \
	src/ServiceWUnderground.cpp \
	src/ServiceYahooWeather.cpp \
	src/Settings.cpp \
	src/SystemTrayWidget.cpp \
	src/main.cpp

FORMS += \
	src/PreferencesDialog.ui

contains(DEFINES, WITH_WEATHER_ICONS) {
	RESOURCES += share/weather-icons.qrc
}

unix {
	isEmpty(PREFIX): PREFIX = /usr/local
	target.path = $$PREFIX/bin
	INSTALLS += target
}
