// Windy - main.cpp
// Copyright (c) 2015 Arkadiusz Bokowy
//
// This file is a part of Windy.
//
// This projected is licensed under the terms of the MIT license.

#include "Application.h"

#if defined(Q_OS_WIN) && !defined(_DLL)
// This sections allows static build on the Windows platform using MSVC++
// compiler. During the non-DLL build the _DLL is not defined. In such a
// case we will enforce plug-ins to be loaded, which in turn will generate
// linking errors if one omits them.
#include <QtPlugin>
Q_IMPORT_PLUGIN(QSvgIconPlugin)
Q_IMPORT_PLUGIN(QSvgPlugin)
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif // Q_OS_WIN && !_DLL

int main(int argc, char *argv[]) {
	Application app(argc, argv);
	return app.exec();
}
