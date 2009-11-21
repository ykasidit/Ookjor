#ifndef OOKJORENGINE_H
#define OOKJORENGINE_H

#include <QList>
#include <stdint.h>
#include <QString>
#include <QThread>
#include <QMutex>
#include <QObject>


class OokjorEngine : public QObject
{
    Q_OBJECT

    public:
    OokjorEngine();
    ~OokjorEngine();

    signals:
    void SearchCompleteSignal(int res);

    public:
    ///////////search        
        class TBtDevInfo
        {
            public:
            QString iName;
            uint8_t iAddr[6];
        };
        class CSearchThread : public QThread
        {
         public:
             CSearchThread(OokjorEngine &aFather):iFather(aFather){}
             void run();
             OokjorEngine &iFather;
        };
        friend class CSearchThread;

        bool StartSearch();
        void CancelSearch();
        void GetDevListClone(QList<TBtDevInfo>& aDevList);
    /////////////////

    ////utils
        static void CopyBDADDR(uint8_t* src,uint8_t* dst);
    /////////

private:

    ////////////////////////for shared stuff between current and result thread like iDevlist
    QThread* iThread;
    QMutex iMutex;
    QList<TBtDevInfo> iDevList;
    //////////////////////////////

};

#endif // OOKJORENGINE_H
