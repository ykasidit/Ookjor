#ifndef OOKJORBLUEZENGINE_H
#define OOKJORBLUEZENGINE_H



#include <QPixmap>
#include <QList>
#include <QString>
#include <QThread>
#include <QMutex>


#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "ookjorengine.h"

class OokjorBlueZEngine : public OokjorEngine
{
    Q_OBJECT

    public:
    OokjorBlueZEngine(QWidget* aParentWindow);
    virtual ~OokjorBlueZEngine();

    signals:
    virtual void EngineStateChangeSignal(int aState);
    virtual void EngineStatusMessageSignal(QString str);

    void GotNewJpgSignal();

    public slots:
    void EngineStateChangeSlot(int aState);

    public:
    ///////////search


        class CBtEngineThread : public QThread
        {
            public:
            CBtEngineThread(OokjorBlueZEngine &aFather):iFather(aFather){}
            OokjorBlueZEngine &iFather;
        };

        class CSearchThread : public CBtEngineThread
        {
         public:
             CSearchThread(OokjorBlueZEngine &aFather):CBtEngineThread(aFather){}
             void run();
        };

        class CSDPThread : public CBtEngineThread
        {
         public:
             CSDPThread(OokjorBlueZEngine &aFather):CBtEngineThread(aFather){}
             void run();
        };

        class CRFCOMMThread : public CBtEngineThread
        {
         public:
             CRFCOMMThread(OokjorBlueZEngine &aFather):CBtEngineThread(aFather){}
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

     void OnNewJpgData(QByteArray& ba);
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

#endif // OOKJORBLUEZENGINE_H
