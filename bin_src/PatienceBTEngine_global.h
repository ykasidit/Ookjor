/*
    Copyright (C) 2009 Kasidit Yusuf.

    This file is part of PatienceBTEngine.

    PatienceBTEngine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PatienceBTEngine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PatienceBTEngine.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef PATIENCEBTENGINE_GLOBAL_H
#define PATIENCEBTENGINE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PATIENCEBTENGINE_LIBRARY)
#  define PATIENCEBTENGINESHARED_EXPORT Q_DECL_EXPORT
#else
#  define PATIENCEBTENGINESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PATIENCEBTENGINE_GLOBAL_H
