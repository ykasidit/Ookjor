#include "ookjorengine.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>


OokjorEngine::OokjorEngine()
{


}

OokjorEngine::~OokjorEngine()
{
    if(iThread && iThread->isRunning())
        iThread->wait();

    delete iThread;
}

void OokjorEngine::CopyBDADDR(uint8_t* src,uint8_t* dst)
{
    int i;
    for(i=0;i<6;i++)
        dst[i] = src[i];
}


///////////search

void OokjorEngine::CSearchThread::run()
{
    //////////code adapted from http://people.csail.mit.edu/albert/bluez-intro/c404.html
    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    char addr[19] = { 0 };
    char name[248] = { 0 };

    iFather.iMutex.lock();
    iFather.iDevList.clear();//clear list in engine class
    iFather.iMutex.unlock();

    dev_id = hci_get_route(NULL);
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        qDebug("open socket failed");
         emit iFather.SearchCompleteSignal(-1);
        return;
    }

    len  = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) perror("hci_inquiry");

    for (i = 0; i < num_rsp; i++) {
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name),
            name, 0) < 0)
        strcpy(name, "[unknown]");

        TBtDevInfo devinfo;
        CopyBDADDR((uint8_t*)addr,(uint8_t*)devinfo.iAddr);
        devinfo.iName = name;

        iFather.iMutex.lock();
        iFather.iDevList.append(devinfo);
        iFather.iMutex.unlock();

        qDebug("%s  %s\n", addr, name);
    }

    free( ii );
    close( sock );
    emit iFather.SearchCompleteSignal(1);
    return;
    ////////////////////////////
}

bool OokjorEngine::StartSearch()
{
    if(iThread && iThread->isRunning())
    {
        return false;
    }

    //if comes here measn thread has finished
    delete iThread;

    iThread = new CSearchThread(*this);
    iThread->start();

    return true;
}

void OokjorEngine::CancelSearch()
{

}

void OokjorEngine::GetDevListClone(QList<TBtDevInfo>& aDevList)
{
    iMutex.lock();
    aDevList = iDevList;
    iMutex.unlock();
}
/////////////////
