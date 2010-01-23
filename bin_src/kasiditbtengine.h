#ifndef KASIDITBTENGINE_H
#define KASIDITBTENGINE_H

#include <QObject>

#include <stdint.h>

class KasiditBTEngine : public QObject
{
    Q_OBJECT

    public:
    virtual ~KasiditBTEngine(){}

    enum TClearEvoBTEngineState
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

    class TBtDevInfo
        {
            public:
            QString iName;
            uint8_t iAddr[6];
            QString iAddrStr;
        };

    virtual void StartPrevdev(QByteArray& ba)=0;
    virtual bool StartSearch()=0;
    virtual void StartSDPToSelectedDev(int aSelIndex)=0;
    virtual void GetDevListClone(QList<TBtDevInfo>& aDevList)=0;
    virtual void Disconnect()=0;

    public:
    virtual void EngineStateChangeSignal(int aState)=0;
    virtual void EngineStatusMessageSignal(QString str)=0;

    public:
    //utils
    static void CopyBDADDR(uint8_t* src,uint8_t* dst)
    {
        int i;
        for(i=0;i<6;i++)
            dst[i] = src[i];
    }
    //
 };


#endif // KASIDITBTENGINE_H
