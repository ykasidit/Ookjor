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

#include "ookjorengine.h"
#include "kasiditbluezengine.h"

//http://en.wikipedia.org/wiki/JPEG
const uint8_t KJpgHeader[] = {0xFF,0xD8};
const uint8_t KJpgFooter[] = {0xFF,0xD9};

const int KMaxInBufferLen = 1024*1024*2;

// Ookjor mobile service id 0x2BC2B92E,0x399211DC, 0x83140800, 0x200C9A38
uint8_t KOokjorSvc_uuid_int[] = {0x2B,0xC2,0xB9,0x2E,0x39,0x92,0x11,0xDC,0x83,0x14,0x08,0x00,0x20,0x0C,0x9A,0x38};

OokjorEngine::OokjorEngine(QWidget* aParentWindow)
{
    iKJpgHeader.append((const char*)KJpgHeader,2);
    iKJpgFooter.append((const char*)KJpgFooter,2);

    iBTEngine = NULL;
    iBTEngine = new KasiditBlueZEngine(KOokjorSvc_uuid_int,aParentWindow);

    if(iBTEngine)
    {
    QObject::connect(iBTEngine, SIGNAL(RFCOMMDataReceivedSignal(QByteArray)),this, SLOT (RFCOMMDataReceivedSlot(QByteArray)));
    }
}

KasiditBTEngine* OokjorEngine::GetBTEngine()
{
    return iBTEngine;
}

OokjorEngine::~OokjorEngine()
{
    delete iBTEngine;
}

void OokjorEngine::RFCOMMDataReceivedSlot(QByteArray ba)
{
    int jpgstartindex,jpgendindex;

    if(iJpgbuff.length() > KMaxInBufferLen)
        iJpgbuff.clear();

    iJpgbuff.append(ba);

    //find and report all jpgs found

    while(true)
    {
        //perror("finding jpg in buffer");

        jpgstartindex = iJpgbuff.indexOf(iKJpgHeader);
        jpgendindex = iJpgbuff.indexOf(iKJpgFooter);

        if(jpgstartindex>=0 && jpgendindex > jpgstartindex)
        {
            QByteArray ajpg = iJpgbuff.mid(jpgstartindex,jpgendindex-jpgstartindex);
            iNewJpgBuffer = ajpg;
            emit GotNewJpgSignal();
            iJpgbuff.remove(0,jpgendindex+1);
            //perror("found jpeg");
        }
        else
        {
            //perror("no more jpeg");
            break;
        }

    }

}
