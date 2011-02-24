/*
    QCANObserver - A CAN Sniffer
    Copyright (C) <2009>  <Netseal>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef CONFIG_H
#define CONFIG_H

#ifndef CANDEVICE_H
#include <QtGlobal>
#include <qwt_global.h>
#endif

//#define WINDOWS
#define LINUX


#if QT_VERSION < 0x040500 // needs Qt 4.5.0 or better
#error "Please use a more recent version of Qt"
#endif


#endif // CONFIG_H
