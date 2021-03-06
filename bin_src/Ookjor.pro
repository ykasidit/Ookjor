# Copyright (C) 2009 Kasidit Yusuf.
# This file is part of Ookjor.
# Ookjor is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# Ookjor is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with Ookjor. If not, see <http://www.gnu.org/licenses/>.
# -------------------------------------------------
# Project created by QtCreator 2009-11-21T06:54:37
# -------------------------------------------------
TARGET = Ookjor
TEMPLATE = app
SOURCES += main.cpp \
    ookjorwindow.cpp \
    selectphonedialog.cpp \
    aboutookjordialog.cpp \
    ookjorengine.cpp

HEADERS += ookjorwindow.h \
    ookjorengine.h \
    selectphonedialog.h \
    aboutookjordialog.h


FORMS += ookjorwindow.ui \
    selectphonedialog.ui \
    aboutookjordialog.ui

# add library search path for local folder (where we store the libPatienceBTEngine.so stuff)
unix: INCLUDEPATH += /usr/include/glib-2.0
unix {
LIBS += -L.
LIBS += -lbluetooth \
        -lPatienceBTEngine
    }

RESOURCES += ookjor_res.qrc
