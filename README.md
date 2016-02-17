Windy [![Build Status](https://travis-ci.org/Arkq/windy.svg?branch=master)](https://travis-ci.org/Arkq/windy)
=====

This application is a minimalistic, system tray based, current weather condition indicator. It is
written purely in Qt without any external dependencies.

All major desktop environments (Gnome and KDE to be exact) have their own weather condition
extensions or widgets. However, they are not compatible with any other simplistic window managers
like [awesome](http://awesome.naquadah.org/) or [fluxbox](http://fluxbox.org/). So, if you are one
of those few, who likes productivity and simplicity, you are forced to look outside your window
(the real one!) to check whatever it is raining or it is not. Even [LXDE](http://lxde.org/) lacks
such a handy feature. This application does not rely on any non-standard ways of placing itself in
the notification area and is not dependent on any bulky library (except Qt... - sorry GTK fans).
It should work on any operating system supported by the Qt framework, which has system tray icon
area available. If you are brave enough, you can compile and run this application on Windows as
well (see screenshots below).

![Screenshot1](/../screenshots/screenshots/win10-windy-clear.png?raw=true)
![Screenshot2](/../screenshots/screenshots/win10-windy-clear-night.png?raw=true)

Weather condition sources
-------------------------

1. [Google search results page](https://www.google.com/search?q=weather)
2. [The Weather Channel](http://www.weather.com/) via its underground weather API ([API
	 key](http://www.wunderground.com/weather/api) is required)
3. [Yahoo Weather](https://weather.yahoo.com/) via its weather RSS

**Note:** It seems, that the main and the only data provider for all available weather condition
reporting services is The Weather Channel. However, there are some discrepancies between data
obtained from Google search results page and from the Underground Weather API itself. So, it is up
to the user, which data source he/she chooses.


Compilation
-----------

	$ mkdir build && cd build
	$ qmake ..
	$ make && make install
