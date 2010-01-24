#ifndef KASIDITBTENGINE_H
#define KASIDITBTENGINE_H

#include <QObject>

#include <stdint.h>

class KasiditBTEngine : public QObject
{
    Q_OBJECT

    public:
    KasiditBTEngine(const uint8_t* aSvc_uuid_int){iSvc_uuid_int = aSvc_uuid_int;}
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
    virtual void RFCOMMDataReceivedSignal(QByteArray ba)=0;

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
 };


#endif // KASIDITBTENGINE_H
