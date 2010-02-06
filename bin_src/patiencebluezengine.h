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


#ifndef PatienceBLUEZENGINE_H
#define PatienceBLUEZENGINE_H

#include "patiencebtengine.h"

#include <QList>
#include <QString>
#include <QThread>
#include <QMutex>

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "patiencebtthreadedengine.h"

class PatienceBlueZEngine : public PatienceBTThreadedEngine
{
    Q_OBJECT

    public:
    PatienceBlueZEngine(MPatineceBTEngineCaller& aCaller, const uint8_t* aSvc_uuid_int);
    virtual ~PatienceBlueZEngine();

        virtual void DoSearch();
        virtual void DoSDP();
        virtual void DoRFCOMM();

        virtual void DoDisconnect();

        struct sockaddr_rc addr;
             int s, status;
};

#endif // PatienceBLUEZENGINE_H
