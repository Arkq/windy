# Windy - windy.pro
# Copyright (c) 2015-2018 Arkadiusz Bokowy

TEMPLATE = app

TARGET = windy
VERSION = 1.0.0

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
	BINDIR = $$PREFIX/bin

	target.path = $$BINDIR
	utils.path = $$BINDIR
	utils.files = utils/icm-meteo

	INSTALLS += target utils
}
