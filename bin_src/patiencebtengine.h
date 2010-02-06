
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

#ifndef PatienceBTENGINE_H
#define PatienceBTENGINE_H

#include "PatienceBTEngine_global.h"

#include <QObject>
#include <stdint.h>


class PatienceBTClientEngine;

class TBtDevInfo
    {
        public:
        QString iName;
        uint8_t iAddr[6];
        QString iAddrStr;
    };

class MPatineceBTEngineCaller
{
    public:
    virtual void OnSelectBtDevice(QList<TBtDevInfo>& aList, int& aReturnSelectedIndex)=0;
};

class PatienceBTClientEngine : public QObject
{
    Q_OBJECT

    public:
    PatienceBTClientEngine(MPatineceBTEngineCaller& aCaller, const uint8_t* aSvc_uuid_int);
    virtual ~PatienceBTClientEngine(){}

    enum TPatienceBTEngineeState
    {
        EBtIdle,
        EBtSearching,
        EBtSelectingPhoneToSDP,
        EBtSearchingSDP,
        EBtSearchingSDPDone,
        EBtConnectingRFCOMM,
        EBtConnectionActive,
        EBtDisconnected
    };

    enum TPatinceBTClientError
    {
        ENoLocalBTDriver,
        ENoLocalBTRadio,
        EInquiryFoundNoDevices,
        EServiceNotFoundOnDevice
    };


    virtual void StartPrevdev(QByteArray& ba)=0;
    virtual bool StartSearch()=0;
    virtual void StartSDPToSelectedDev(int aSelIndex)=0;
    virtual void GetDevListClone(QList<TBtDevInfo>& aDevList)=0;
    virtual void Disconnect()=0;

    /////////////signals
    public:
    virtual void EngineErrorSignal(int aError)=0;//TPatinceBTClientError
    virtual void EngineStateChangeSignal(int aState)=0;
    virtual void EngineStatusMessageSignal(QString str)=0;
    virtual void RFCOMMDataReceivedSignal(QByteArray ba)=0;
    ///////////////

    public:
    //utils
    static void CopyBDADDR(uint8_t* src,uint8_t* dst)
    {
        int i;
        for(i=0;i<6;i++)
            dst[i] = src[i];
    }
    //

    protected:

    //128 bit uuid of service to connect to, for example - in Ookjor program: {0x2B,0xC2,0xB9,0x2E,0x39,0x92,0x11,0xDC,0x83,0x14,0x08,0x00,0x20,0x0C,0x9A,0x38}
    const uint8_t* iSvc_uuid_int;

    MPatineceBTEngineCaller& iCaller;
 };


#endif // PatienceBTENGINE_H
