#ifndef KASIDITBLUEZENGINE_H
#define KASIDITBLUEZENGINE_H

#include "kasiditbtengine.h"

#include <QList>
#include <QString>
#include <QThread>
#include <QMutex>

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

class KasiditBlueZEngine : public KasiditBTEngine
{
    Q_OBJECT

    public:
    KasiditBlueZEngine(const uint8_t* aSvc_uuid_int, QWidget* aParentWindow);
    virtual ~KasiditBlueZEngine();

    signals:
    virtual void EngineStateChangeSignal(int aState);
    virtual void EngineStatusMessageSignal(QString str);    
    virtual void RFCOMMDataReceivedSignal(QByteArray ba);

    public slots:
    void EngineStateChangeSlot(int aState); // to detect/handle our own state change made by bt operation threads

    public:

    ///////////bt operation thread classes and functions

        class CBtEngineThread : public QThread
        {
            public:
            CBtEngineThread(KasiditBlueZEngine &aFather):iFather(aFather){}
            KasiditBlueZEngine &iFather;
        };

        class CSearchThread : public CBtEngineThread
        {
         public:
             CSearchThread(KasiditBlueZEngine &aFather):CBtEngineThread(aFather){}
             void run();
        };

        class CSDPThread : public CBtEngineThread
        {
         public:
             CSDPThread(KasiditBlueZEngine &aFather):CBtEngineThread(aFather){}
             void run();
        };

        class CRFCOMMThread : public CBtEngineThread
        {
         public:
             CRFCOMMThread(KasiditBlueZEngine &aFather):CBtEngineThread(aFather){}
             void run();

             struct sockaddr_rc addr;
             int s, status;
        };


        friend class CSearchThread;
        friend class CSDPThread;

        virtual void StartPrevdev(QByteArray& ba);
        virtual bool StartSearch();
        virtual void StartSDPToSelectedDev(int aSelIndex);
        virtual void GetDevListClone(QList<TBtDevInfo>& aDevList);
        virtual void Disconnect();
    /////////////////

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

#endif // KASIDITBLUEZENGINE_H
