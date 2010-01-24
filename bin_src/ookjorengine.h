/*
    Copyright (C) 2009 Kasidit Yusuf.

    This file is part of Ookjor.

    Ookjor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ookjor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Ookjor.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OOKJORENGINE_H
#define OOKJORENGINE_H

#include <QObject>
#include "kasiditbtengine.h"

class OokjorEngine : public QObject
{    
    Q_OBJECT

    public:
    OokjorEngine(QWidget* aParentWindow);
    ~OokjorEngine();
    QByteArray iNewJpgBuffer;
    KasiditBTEngine* GetBTEngine();

    signals:
    void GotNewJpgSignal();

    protected slots:
    void RFCOMMDataReceivedSlot(QByteArray ba);

    protected:
    KasiditBTEngine* iBTEngine;
    QByteArray iJpgbuff;
    QByteArray iKJpgHeader,iKJpgFooter;

    //QWidget* iParentWindow;
};



#endif // OOKJORENGINE_H
