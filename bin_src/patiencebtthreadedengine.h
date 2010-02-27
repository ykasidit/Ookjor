#ifndef PATIENCEBTTHREADEDENGINE_H
#define PATIENCEBTTHREADEDENGINE_H

#include <QList>
#include <QString>
#include <QThread>
#include <QMutex>

#include "patiencebtengine.h"

class PatienceBTThreadedEngine : public PatienceBTClientEngine
{
    Q_OBJECT

    public:
    PatienceBTThreadedEngine(MPatineceBTEngineCaller& aCaller, const uint8_t* aSvc_uuid_int);
    virtual ~PatienceBTThreadedEngine();

    signals:
    virtual void EngineStateChangeSignal(int aState);
    virtual void EngineStatusMessageSignal(QString str);
    virtual void RFCOMMDataReceivedSignal(QByteArray ba);
    virtual void EngineErrorSignal(int aError);

    public slots:
    void EngineStateChangeSlot(int aState); // to detect/handle our own state change made by bt operation threads

    public:

    ///////////bt operation thread classes and functions

        class CBtEngineThread : public QThread
        {
            public:
            CBtEngineThread(PatienceBTThreadedEngine &aFather):iFather(aFather){}
            PatienceBTThreadedEngine &iFather;
        };

        class CSearchThread : public CBtEngineThread
        {
         public:
             CSearchThread(PatienceBTThreadedEngine &aFather):CBtEngineThread(aFather){}
             void run();
        };

        class CSDPThread : public CBtEngineThread
        {
         public:
             CSDPThread(PatienceBTThreadedEngine &aFather):CBtEngineThread(aFather){}
             void run();
        };

        class CRFCOMMThread : public CBtEngineThread
        {
         public:
             CRFCOMMThread(PatienceBTThreadedEngine &aFather):CBtEngineThread(aFather){}
             void run();             
        };

        //to be implemented for each bt stack - called by threads
        virtual void DoSearch()=0;
        virtual void DoSDP()=0;
        virtual void DoRFCOMM()=0;
        virtual void DoDisconnect()=0;        
        ////////////////////////

        virtual void Disconnect();

        friend class CSearchThread;
        friend class CSDPThread;

        virtual void StartPrevdev(QByteArray& ba);
        virtual bool StartSearch();
        virtual void StartSDPToSelectedDev(int aSelIndex);
        virtual void GetDevListClone(QList<TBtDevInfo>& aDevList);

    /////////////////

protected:

    ////////////////////////for shared stuff between current and result thread like iDevlist
    QThread* iThread;
    QMutex iMutex;
    QList<TBtDevInfo> iDevList;
    int iRFCOMMChannel;
    int iLiveSocketToDisconnect;
    //////////////////////////////

    int iSelectedIndex;
};

#endif // PATIENCEBTTHREADEDENGINE_H
