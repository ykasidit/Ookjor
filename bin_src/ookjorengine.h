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

#include <QList>
#include <stdint.h>
#include <QString>
#include <QThread>
#include <QMutex>
#include <QObject>

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <QPixmap>



class OokjorEngine : public QObject
{
    Q_OBJECT

    public:
    OokjorEngine(QWidget* aParentWindow);
    ~OokjorEngine();

    enum TOokjorBTEngineState
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

    signals:
    void EngineStateChangeSignal(int aState);
    void EngineStatusMessageSignal(QString str);
    void GotNewJpgSignal();

    public slots:
    void EngineStateChangeSlot(int aState);

    public:
    ///////////search        
        class TBtDevInfo
        {
            public:
            QString iName;
            uint8_t iAddr[6];
            QString iAddrStr;
        };

        class CBtEngineThread : public QThread
        {
            public:
            CBtEngineThread(OokjorEngine &aFather):iFather(aFather){}
            OokjorEngine &iFather;
        };

        class CSearchThread : public CBtEngineThread
        {
         public:
             CSearchThread(OokjorEngine &aFather):CBtEngineThread(aFather){}
             void run();             
        };        

        class CSDPThread : public CBtEngineThread
        {
         public:
             CSDPThread(OokjorEngine &aFather):CBtEngineThread(aFather){}
             void run();                          
        };

        class CRFCOMMThread : public CBtEngineThread
        {
         public:
             CRFCOMMThread(OokjorEngine &aFather):CBtEngineThread(aFather){}
             void run();

             struct sockaddr_rc addr;
             int s, status;
        };


        friend class CSearchThread;
        friend class CSDPThread;

        void StartPrevdev(QByteArray& ba);
        bool StartSearch();
        void CancelSearch();
        void StartSDPToSelectedDev(int aSelIndex);
        void GetDevListClone(QList<TBtDevInfo>& aDevList);
        void Disconnect();
    /////////////////

    ////utils
        static void CopyBDADDR(uint8_t* src,uint8_t* dst);
    /////////

        void OnNewJpgData(QByteArray& ba);
        QByteArray iNewJpgBuffer;
private:

    ////////////////////////for shared stuff between current and result thread like iDevlist
    QThread* iThread;
    QMutex iMutex;
    QList<TBtDevInfo> iDevList;
    int iRFCOMMChannel;    
    int iLiveSocketToDisconnect;
    //////////////////////////////

    int iSelectedIndex;

    QWidget* iParentWindow;

};

#endif // OOKJORENGINE_H
