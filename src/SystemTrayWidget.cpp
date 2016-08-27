// Windy - SystemTrayWidget.cpp
// Copyright (c) 2015-2016 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This project is licensed under the terms of the MIT license.

#include "SystemTrayWidget.h"

#include <QBuffer>

#include "Settings.h"


SystemTrayWidget::SystemTrayWidget(QObject *parent) :
		QObject(parent),
		m_action_refresh(QIcon::fromTheme("view-refresh"), tr("&Refresh"), parent),
		m_action_preferences(QIcon::fromTheme("document-properties"), tr("&Preferences"), parent),
		m_action_about(QIcon::fromTheme("help-about"), tr("&About"), parent),
		m_action_quit(QIcon::fromTheme("application-exit"), tr("&Quit"), parent) {

	// assembly widget's context menu
	m_tray_icon.setContextMenu(&m_context_menu);
	m_context_menu.addAction(&m_action_refresh);
	m_context_menu.addAction(&m_action_preferences);
	m_context_menu.addAction(&m_action_about);
	m_context_menu.addSeparator();
	m_context_menu.addAction(&m_action_quit);

	connect(&m_tray_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			this, SLOT(dispatchIconActivation(QSystemTrayIcon::ActivationReason)));

	connect(&m_action_refresh, SIGNAL(triggered()), SLOT(dispatchMenuAction()));
	connect(&m_action_preferences, SIGNAL(triggered()), SLOT(dispatchMenuAction()));
	connect(&m_action_about, SIGNAL(triggered()), SLOT(dispatchMenuAction()));
	connect(&m_action_quit, SIGNAL(triggered()), SLOT(dispatchMenuAction()));

	// initialize tray icon to the default value
	m_conditions.icon = WeatherConditions::WeatherIcon::Clear;
	updateIcon();

	m_tray_icon.setToolTip(tr("Windy - Minimalistic weather indicator"));
	m_tray_icon.show();

}

void SystemTrayWidget::setWeatherConditions(const WeatherConditions &conditions) {

	dumpWeatherConditions(conditions);

	// If the location name is not set, it will most likely mean, that the request
	// has failed - it might be a temporary network connection failure, or a 3rd
	// party service malfunction. In such a case do not update our widget.
	if (!conditions.locationName.isEmpty()) {

		// update our internal weather conditions structure
		m_conditions = conditions;

		updateIcon();
		updateToolTip();

	}

}

void SystemTrayWidget::updateIcon() {

	QStringList icons;
	QString fallback;

	switch (m_conditions.icon) {
	case WeatherConditions::WeatherIcon::Clear:
		fallback = ":/icons/weather-clear";
		icons << "weather-clear-symbolic";
		icons << "weather-clear";
		break;
	case WeatherConditions::WeatherIcon::ClearNight:
		fallback = ":/icons/weather-clear-night";
		icons << "weather-clear-night-symbolic";
		icons << "weather-night-clear";
		break;
	case WeatherConditions::WeatherIcon::FewClouds:
		fallback = ":/icons/weather-few-clouds";
		icons << "weather-few-clouds-symbolic";
		icons << "weather-few-clouds";
		break;
	case WeatherConditions::WeatherIcon::FewCloudsNight:
		fallback = ":/icons/weather-few-clouds-night";
		icons << "weather-few-clouds-night-symbolic";
		icons << "weather-few-clouds-night";
		break;
	case WeatherConditions::WeatherIcon::Overcast:
		fallback = ":/icons/weather-overcast";
		icons << "weather-overcast-symbolic";
		icons << "weather-overcast";
		break;
	case WeatherConditions::WeatherIcon::Showers:
		fallback = ":/icons/weather-showers";
		icons << "weather-showers-symbolic";
		icons << "weather-showers";
		break;
	case WeatherConditions::WeatherIcon::ShowersScattered:
		fallback = ":/icons/weather-showers";
		icons << "weather-showers-scattered-symbolic";
		icons << "weather-showers-scattered";
		break;
	case WeatherConditions::WeatherIcon::Fog:
		fallback = ":/icons/weather-fog";
		icons << "weather-fog-symbolic";
		icons << "weather-fog";
		break;
	case WeatherConditions::WeatherIcon::Snow:
		fallback = ":/icons/weather-snow";
		icons << "weather-snow-symbolic";
		icons << "weather-snow";
		break;
	case WeatherConditions::WeatherIcon::Storm:
		fallback = ":/icons/weather-storm";
		icons << "weather-storm-symbolic";
		icons << "weather-storm";
		break;
	case WeatherConditions::WeatherIcon::SevereAlert:
		fallback = ":/icons/weather-severe-alert";
		icons << "weather-severe-alert-symbolic";
		icons << "weather-severe-alert";
		break;
	}

// By default, only X11 supports themed icons, and we are not going to
// install our theme on the user's operating system. However, we will
// ship bundled resources for systems other than Linux.
#ifdef Q_OS_LINUX

	// iterate over icon candidates and set the first one which is available
	for (QStringList::ConstIterator it = icons.begin(); it != icons.end(); it++) {
		QIcon icon(QIcon::fromTheme(*it));
		if (!icon.isNull()) {
			m_tray_icon.setIcon(icon);
			return;
		}
	}

	qWarning() << "Weather status icon not found:" << icons;

#endif // Q_OS_LINUX

	// NOTE: If themed icon was not found and windy was build without bundled
	//       icon pack, we might end up with a "blank" tray icon. Such a case
	//       should be treated as a misconfiguration, not a bug.
	m_tray_icon.setIcon(QIcon(fallback));
}

void SystemTrayWidget::updateToolTip() {

	Settings *settings = Settings::settings();

	QString temperature(getLabelNotAvailable());
	QString pressure(getLabelNotAvailable());
	QString windSpeed(getLabelNotAvailable());
	QString windGustSpeed(getLabelNotAvailable());
	QString windDirection(getLabelNotAvailable());
	QString windChill(getLabelNotAvailable());
	QString humidity(getLabelNotAvailable());
	QString dewPoint(getLabelNotAvailable());
	QString visibility(getLabelNotAvailable());

	if (m_conditions.windDirection != -1)
		windDirection = getLabelWindDirection(m_conditions.windDirection);
	if (m_conditions.humidity != -1)
		humidity.setNum(m_conditions.humidity, 'f', 0);

	switch (settings->getUnitTemperature()) {
	case Settings::UnitTemperature::Celsius:
		if (m_conditions.temperature != -1)
			temperature.setNum(m_conditions.temperature - 273.15, 'f', 1);
		if (m_conditions.windChill != -1)
			windChill.setNum(m_conditions.windChill - 273.15, 'f', 1);
		if (m_conditions.dewPoint != -1)
			dewPoint.setNum(m_conditions.dewPoint - 273.15, 'f', 1);
		break;
	case Settings::UnitTemperature::Fahrenheit:
		if (m_conditions.temperature != -1)
			temperature.setNum((m_conditions.temperature - 273.15) * 1.8 + 32, 'f', 1);
		if (m_conditions.windChill != -1)
			windChill.setNum((m_conditions.windChill - 273.15) * 1.8 + 32, 'f', 1);
		if (m_conditions.dewPoint != -1)
			dewPoint.setNum((m_conditions.dewPoint - 273.15) * 1.8 + 32, 'f', 1);
		break;
	case Settings::UnitTemperature::Kelvin:
		if (m_conditions.temperature != -1)
			temperature.setNum(m_conditions.temperature, 'f', 1);
		if (m_conditions.windChill != -1)
			windChill.setNum(m_conditions.windChill, 'f', 1);
		if (m_conditions.dewPoint != -1)
			dewPoint.setNum(m_conditions.dewPoint, 'f', 1);
		break;
	}

	switch (settings->getUnitPressure()) {
	case Settings::UnitPressure::Hectopascal:
		if (m_conditions.pressure != -1)
			pressure.setNum(m_conditions.pressure / 100, 'f', 0);
		break;
	case Settings::UnitPressure::PoundPerSquareInch:
		if (m_conditions.pressure != -1)
			pressure.setNum(m_conditions.pressure / 6894.75729, 'f', 1);
		break;
	case Settings::UnitPressure::MillimeterOfMercury:
		if (m_conditions.pressure != -1)
			pressure.setNum(m_conditions.pressure / 133.3224, 'f', 0);
		break;
	}

	switch (settings->getUnitWindSpeed()) {
	case Settings::UnitWindSpeed::KilometerPerHour:
		if (m_conditions.windSpeed != -1)
			windSpeed.setNum(m_conditions.windSpeed * 3.6, 'f', 0);
		if (m_conditions.windGustSpeed != -1)
			windGustSpeed.setNum(m_conditions.windGustSpeed * 3.6, 'f', 0);
		if (m_conditions.visibility != -1)
			visibility.setNum(m_conditions.visibility / 1000, 'f', 1);
		break;
	case Settings::UnitWindSpeed::MilePerHour:
		if (m_conditions.windSpeed != -1)
			windSpeed.setNum(m_conditions.windSpeed / 0.44704, 'f', 0);
		if (m_conditions.windGustSpeed != -1)
			windGustSpeed.setNum(m_conditions.windGustSpeed / 0.44704, 'f', 0);
		if (m_conditions.visibility != -1)
			visibility.setNum(m_conditions.visibility / 1609.3472, 'f', 1);
		break;
	case Settings::UnitWindSpeed::MeterPerSecond:
		if (m_conditions.windSpeed != -1)
			windSpeed.setNum(m_conditions.windSpeed, 'f', 1);
		if (m_conditions.windGustSpeed != -1)
			windGustSpeed.setNum(m_conditions.windGustSpeed, 'f', 1);
		if (m_conditions.visibility != -1)
			visibility.setNum(m_conditions.visibility, 'f', 0);
		break;
	}

	// trim location name if it exceeds 30 characters (prevent wrapping)
	QString locationName(m_conditions.locationName);
	if (locationName.length() > 30) {

		// strip all non-alphanumerical characters from the trimmed end
		auto i = locationName.constBegin() + 28;
		for (; i != locationName.constBegin() - 1; i--)
			if ((*i).isLetterOrNumber())
				break;

		locationName = locationName.left(i - locationName.constBegin() + 1);
		if (locationName.isEmpty())
			// it has turned out that we've exterminated all characters from
			// the location name string, so restore the original text
			locationName = m_conditions.locationName;
		else
			locationName += "...";

	}

	QString messageTemplate(tr("<b>{NAME}</b>"));
	if (settings->getShowTemperature()) {
		if (settings->getShowWindChill())
			messageTemplate += tr("<br/>Temperature: {TEMP} ({CHILL}) {T}");
		else
			messageTemplate += tr("<br/>Temperature: {TEMP} {T}");
	}
	if (settings->getShowWindSpeed()) {
		if (settings->getShowWindGustSpeed())
			messageTemplate += tr("<br/>Wind: {WIND} ({GUST}) {S}");
		else
			messageTemplate += tr("<br/>Wind: {WIND} {S}");
		if (m_conditions.windDirection != -1)
			messageTemplate += tr(" {WDIR}");
	}
	if (settings->getShowVisibility())
		messageTemplate += tr("<br/>Visibility: {VISIB} {D}");
	if (settings->getShowPressure())
		messageTemplate += tr("<br/>Pressure: {PRES} {P}");

// Rich formated text support is available in the X11-like environments only.
// On other platforms, tooltips are displayed using native widgets which lack
// such a functionality.
#ifndef Q_OS_LINUX
	messageTemplate.replace("<br/>", "\n");
	messageTemplate.remove(QRegExp("<[^>]*>"));
	messageTemplate = messageTemplate.trimmed();
#endif // Q_OS_LINUX

#ifdef Q_OS_LINUX
	if (settings->getShowTooltipIcon()) {
		QByteArray img;
		QBuffer buffer(&img);
		QString tableTemplate(tr("<table><tr><td valign=middle>{IMG}</td><td>{MSG}</td></tr></table>"));

		m_tray_icon.icon().pixmap(32, 32).save(&buffer, "PNG");
		messageTemplate = tableTemplate
			.replace("{IMG}", QString("<img src='data:image/png;base64,%0'>").arg(img.toBase64().data()))
			.replace("{MSG}", messageTemplate);
	}
#endif // Q_OS_LINUX

	m_tray_icon.setToolTip(messageTemplate
			.replace("{T}", getUnitTemperature())
			.replace("{P}", getUnitPressure())
			.replace("{D}", getUnitDistance())
			.replace("{S}", getUnitSpeed())
			.replace("{NAME}", locationName)
			.replace("{TEMP}", temperature)
			.replace("{PRES}", pressure)
			.replace("{WIND}", windSpeed)
			.replace("{GUST}", windGustSpeed)
			.replace("{WDIR}", windDirection)
			.replace("{CHILL}", windChill)
			.replace("{HUMID}", humidity)
			.replace("{DEWPT}", dewPoint)
			.replace("{VISIB}", visibility));

}

void SystemTrayWidget::dispatchIconActivation(QSystemTrayIcon::ActivationReason reason) {
	// NOTE: Icon activation dispatcher handles left and middle click only. Other
	//       actions are handled internally by this widget, so there is no reason
	//       for them to be exposed.
	if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::MiddleClick)
		emit iconActivated(reason);
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

QString SystemTrayWidget::getLabelWindDirection(float direction) const {

	QStringList labels({
			tr("N"), tr("NW"), tr("W"), tr("SW"),
			tr("S"), tr("SE"), tr("E"), tr("NE"),
	});

	// NOTE: This algorithm will not work correctly for directions with negative
	//       value. Such a restriction is not a problem for us, though.
	return labels[(qRound(direction + 180.0 / labels.size()) * labels.size() / 360) % labels.size()];
}

QString SystemTrayWidget::getUnitDistance() const {

	QString unit;

	switch (Settings::settings()->getUnitWindSpeed()) {
	case Settings::UnitWindSpeed::KilometerPerHour:
		unit = trUtf8("km");
		break;
	case Settings::UnitWindSpeed::MilePerHour:
		unit = trUtf8("mi.");
		break;
	case Settings::UnitWindSpeed::MeterPerSecond:
		unit = trUtf8("m");
		break;
	}

	return unit;
}

QString SystemTrayWidget::getUnitPressure() const {

	QString unit;

	switch (Settings::settings()->getUnitPressure()) {
	case Settings::UnitPressure::Hectopascal:
		unit = trUtf8("hPa");
		break;
	case Settings::UnitPressure::PoundPerSquareInch:
		unit = trUtf8("psi");
		break;
	case Settings::UnitPressure::MillimeterOfMercury:
		unit = trUtf8("mmHg");
		break;
	}

	return unit;
}

QString SystemTrayWidget::getUnitSpeed() const {

	QString unit;

	switch (Settings::settings()->getUnitWindSpeed()) {
	case Settings::UnitWindSpeed::KilometerPerHour:
		unit = trUtf8("km/h");
		break;
	case Settings::UnitWindSpeed::MilePerHour:
		unit = trUtf8("MpH");
		break;
	case Settings::UnitWindSpeed::MeterPerSecond:
		unit = trUtf8("m/s");
		break;
	}

	return unit;
}

QString SystemTrayWidget::getUnitTemperature() const {

	QString unit;

	switch (Settings::settings()->getUnitTemperature()) {
	case Settings::UnitTemperature::Celsius:
		unit = trUtf8("°C");
		break;
	case Settings::UnitTemperature::Fahrenheit:
		unit = trUtf8("°F");
		break;
	case Settings::UnitTemperature::Kelvin:
		unit = trUtf8("K");
		break;
	}

	return unit;
}
