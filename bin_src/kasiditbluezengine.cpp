#include "kasiditbluezengine.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <QMessageBox>
#include "selectphonedialog.h"
 #include <QDialog>

#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>

#include <errno.h> //errno global var that holds the error cause

#include <QFile>

KasiditBlueZEngine::KasiditBlueZEngine(const uint8_t* aSvc_uuid_int, QWidget* aParentWindow):KasiditBTEngine(aSvc_uuid_int)
{
   iParentWindow = aParentWindow;
   QObject::connect(this, SIGNAL(EngineStateChangeSignal(int)),this, SLOT (EngineStateChangeSlot(int)));
    iThread = NULL;
    iLiveSocketToDisconnect = 0;
}

KasiditBlueZEngine::~KasiditBlueZEngine()
{
    if(iLiveSocketToDisconnect!=0)
        Disconnect(); //this would stop running rfcomm thread, if not - in some cases user needs to close the mobile phone's bt to unblock this.

    if(iThread && iThread->isRunning())
        iThread->wait();

    delete iThread;
}


///////////search

void KasiditBlueZEngine::CSearchThread::run()
{
    //////////code adapted from http://people.csail.mit.edu/albert/bluez-intro/c404.html
    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    char addr[19] = { 0 };
    char name[248] = { 0 };

    emit iFather.EngineStateChangeSignal(EBtSearching);

    emit iFather.EngineStatusMessageSignal("Searching...");


    iFather.iMutex.lock();
    iFather.iDevList.clear();//clear list in engine class
    iFather.iMutex.unlock();

    dev_id = hci_get_route(NULL);
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {

         emit iFather.EngineStateChangeSignal(EBtIdle);
         emit iFather.EngineStatusMessageSignal("Open BT socket failed");
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
        CopyBDADDR((uint8_t*)&(ii+i)->bdaddr,(uint8_t*)devinfo.iAddr);
        devinfo.iName = name;
        devinfo.iAddrStr = addr;

        iFather.iMutex.lock();
        iFather.iDevList.append(devinfo);
        iFather.iMutex.unlock();

        QString str;
        str.sprintf("Found %s  (%s), Searching...", addr, name);


        emit iFather.EngineStatusMessageSignal(str);
    }

    free( ii );
    close( sock );

    emit iFather.EngineStatusMessageSignal("Search complete...");


    emit iFather.EngineStateChangeSignal(EBtSelectingPhoneToSDP);
    ////////////////////////////
}

void KasiditBlueZEngine::CSDPThread::run()
{
    perror("entered sdp run");

    emit iFather.EngineStateChangeSignal(EBtSearchingSDP);
    emit iFather.EngineStatusMessageSignal("Searching device...");
    int channel = -1;
    iFather.iMutex.lock();
    iFather.iRFCOMMChannel = channel; //set to invalid
    iFather.iMutex.unlock();

            //adjusted from http://people.csail.mit.edu/albert/bluez-intro/x604.html
            uuid_t svc_uuid;
            int err;
            bdaddr_t target;

            perror("copy bdaddr");
            CopyBDADDR(iFather.iDevList[iFather.iSelectedIndex].iAddr, target.b);
            perror("copy bdaddr complete");

            sdp_list_t *response_list = NULL, *search_list, *attrid_list;
            sdp_session_t *session = 0;

            emit iFather.EngineStatusMessageSignal("Connect to SDP on remote");
            // connect to the SDP server running on the remote machine
            session = sdp_connect( BDADDR_ANY, &target, SDP_RETRY_IF_BUSY );
            if(errno!=0)
            {
            perror("errno not 0 - sdp_connect failed");
            emit iFather.EngineStatusMessageSignal("Failed to connect to \"previously connected device\"");
            emit iFather.EngineStateChangeSignal(EBtSearchingSDPDone);
            sdp_close(session);
            return;
            }

            // specify the UUID of the application we're searching for
            sdp_uuid128_create( &svc_uuid, iFather.iSvc_uuid_int );
            perror("sdp state 1");
            search_list = sdp_list_append( NULL, &svc_uuid );
            perror("sdp state 2");

            // specify that we want a list of all the matching applications' attributes
            uint32_t range = 0x0000ffff;
            attrid_list = sdp_list_append( NULL, &range );
            perror("sdp state 3");

            // get a list of service records that have UUID 0xabcd
            emit iFather.EngineStatusMessageSignal("get a list of service records that has our target UUID");
            err = sdp_service_search_attr_req( session, search_list, \
                    SDP_ATTR_REQ_RANGE, attrid_list, &response_list);

            //parse response
            emit iFather.EngineStatusMessageSignal("Parsing response");
            sdp_list_t *r = response_list;

    // go through each of the service records
    for (; r; r = r->next ) {
        sdp_record_t *rec = (sdp_record_t*) r->data;
        sdp_list_t *proto_list;

        // get a list of the protocol sequences
        if( sdp_get_access_protos( rec, &proto_list ) == 0 ) {
        sdp_list_t *p = proto_list;

        // go through each protocol sequence
        for( ; p ; p = p->next ) {
            sdp_list_t *pds = (sdp_list_t*)p->data;

            // go through each protocol list of the protocol sequence
            for( ; pds ; pds = pds->next ) {

                // check the protocol attributes
                sdp_data_t *d = (sdp_data_t*)pds->data;
                int proto = 0;
                for( ; d; d = d->next ) {
                    switch( d->dtd ) {
                        case SDP_UUID16:
                        case SDP_UUID32:
                        case SDP_UUID128:
                            proto = sdp_uuid_to_proto( &d->val.uuid );
                            break;
                        case SDP_UINT8:
                            if( proto == RFCOMM_UUID ) {
                                qDebug("rfcomm channel: %d\n",d->val.int8);
                                channel = d->val.int8;
                            }
                            break;
                    }
                }
            }
            sdp_list_free( (sdp_list_t*)p->data, 0 );
        }
        sdp_list_free( proto_list, 0 );

        }

        qDebug("found service record 0x%x\n", rec->handle);
        sdp_record_free( rec );
    }

    sdp_close(session);
    iFather.iMutex.lock();
    iFather.iRFCOMMChannel = channel;
    iFather.iMutex.unlock();
    emit iFather.EngineStateChangeSignal(EBtSearchingSDPDone);
}


//adapted from http://people.csail.mit.edu/albert/bluez-intro/x502.html
void KasiditBlueZEngine::CRFCOMMThread::run()
{
    const int KReadBuffSize = 100*1024;//100kb buffer
    uint8_t* buf = (uint8_t*) malloc(KReadBuffSize);
    QByteArray jpgbuff;
    QByteArray qKJpgHeader,qKJpgFooter;

    //http://en.wikipedia.org/wiki/JPEG
    uint8_t KJpgHeader[] = {0xFF,0xD8};
    qKJpgHeader.append((const char*)KJpgHeader,2);
    uint8_t KJpgFooter[] = {0xFF,0xD9};
    qKJpgFooter.append((const char*)KJpgFooter,2);

    memset(&addr,0,sizeof(addr));
    CopyBDADDR(iFather.iDevList[iFather.iSelectedIndex].iAddr, (uint8_t*) &(addr.rc_bdaddr));

    //zeromemory()

    emit iFather.EngineStatusMessageSignal("allocating socket");
    emit iFather.EngineStateChangeSignal(EBtConnectingRFCOMM);

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    iFather.iMutex.lock();
    addr.rc_channel = (uint8_t) iFather.iRFCOMMChannel; //use the rfcomm channel we got through SDP
    iFather.iMutex.unlock();
    //str2ba( dest, &addr.rc_bdaddr ); already copied addr above

    // connect to server
    socklen_t addrlen = sizeof(addr);

    emit iFather.EngineStatusMessageSignal("Preparing connection...");
    emit iFather.EngineStateChangeSignal(EBtConnectingRFCOMM);

    status = ::connect(s, (__const struct sockaddr *)&addr,addrlen );

    /* THAT ISSUE WAS FIXED - all about not using the sdp found rfcomm channel when connecting rfcomm

    doesn't help, first connection still fails since ubuntu 9.10 - same problem when using bt-sendto
    ///test fix "first connect read hangs" on some driver versions - so we disconnect first conn above, wait 2 sec, then connect again
    close(s);
    emit iFather.EngineStatusMessageSignal("Preparing connection stage 2/3...");
    perror("closed s, sleep 2 sec");
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    sleep(2);
    perror("re con s");
    emit iFather.EngineStatusMessageSignal("Preparing connection stage 3/3...");
    status = ::connect(s, (__const struct sockaddr *)&addr,addrlen );
    //////////////
*/
    perror("checking conn status");

    //Read
    if( status == 0 ) {

        //perror("wait 2 sec before read to make sure mobile accepted connection and fully opened its socket"); //otherwise strange blocking read and mobile nondisconnecting issues are observed
        //sleep(2);

        //no need mutex here because the button to disconnect (that would call close on socket handle iLiveSocketToDisconnect) isn't shown yet
         iFather.iMutex.lock();
         iFather.iLiveSocketToDisconnect = s;
         iFather.iMutex.unlock();

         //write bdaddr to file as prevdev.bdaddr
          QFile f( "prevdev.bdaddr" );
          if(f.open(QIODevice::WriteOnly))
          {
              QByteArray baddr((const char*) &(addr.rc_bdaddr),6);
              f.write(baddr);
              f.close();
          }



    emit iFather.EngineStatusMessageSignal("Connected");
    perror("presignal state change 0");
    emit iFather.EngineStateChangeSignal(EBtConnectionActive);
    perror("postsignal state change 0");

        int bytes_read;
        //uint32_t count=1;
        //uint32_t totalb=0;
        //char progress;

        while(true)
        {
            //memset(buf,0,KReadBuffSize);
            //perror("Reading...");
            bytes_read = ::read(s, buf, KReadBuffSize);
            //perror("Read %d bytes",bytes_read);
            if( bytes_read > 0 )
            {
                QByteArray ba((const char*)buf,bytes_read);
                emit iFather.RFCOMMDataReceivedSignal(ba);
            }
            else
            {
                qDebug("readerr %d bytes: %d\n", bytes_read, errno);
                emit iFather.EngineStatusMessageSignal("Disconnected");
                emit iFather.EngineStateChangeSignal(EBtDisconnected);
                break;
            }

            /*
            count++;
            if(count > 10000)
            count = 0;
            */
        }



    }
    else
    if( status < 0 )
        {
            qDebug("open socket failed status %d",status);
            emit iFather.EngineStatusMessageSignal("Connect Failed");
        }

    emit iFather.EngineStateChangeSignal(EBtIdle);

    close(s);
    free(buf);

}

bool KasiditBlueZEngine::StartSearch()
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


void KasiditBlueZEngine::GetDevListClone(QList<TBtDevInfo>& aDevList)
{
    iMutex.lock();
    aDevList = iDevList;
    iMutex.unlock();
}

void KasiditBlueZEngine::Disconnect()
{
    qDebug("preparing to close socket handle %d",iLiveSocketToDisconnect);
    close(iLiveSocketToDisconnect); //thise would cause the CRFCOMMThread to quit as it's waiting on read
    perror("closed socket");
    iLiveSocketToDisconnect = 0;
}

void KasiditBlueZEngine::StartPrevdev(QByteArray& ba)
{

    emit EngineStateChangeSignal(KasiditBlueZEngine::EBtSearching); //let the ui prepare to go to sdp state
    iDevList.clear();

    TBtDevInfo devinfo;
    CopyBDADDR((uint8_t*)ba.data(),(uint8_t*)devinfo.iAddr);

    iDevList.append(devinfo);
    iSelectedIndex = 0;
    StartSDPToSelectedDev(0);
}

void KasiditBlueZEngine::StartSDPToSelectedDev(int aSelIndex)
{
                    iSelectedIndex = aSelIndex;
                    qDebug("user selected index: %d",aSelIndex);
                    QString str;
                    str = iDevList[aSelIndex].iName;
                    str += " selected, preparing to search for service...";
                    emit EngineStatusMessageSignal(str);


                    //////////////// start sdp search thread
                    if(iThread && iThread->isRunning())
                     {
                        perror("Warning: waiting on scan thread? actually no thread should be active now...");
                        iThread->wait();
                        perror("wait thread ended");
                     }

                    //if comes here measn thread has finished
                    perror("deleting iThread");

                    delete iThread;
                    iThread = NULL;

                    iThread = new CSDPThread(*this);
                    perror("created sdp thread");
                    iThread->start();
                    perror("started sdp thread");
                    emit EngineStateChangeSignal(EBtSearchingSDP);
                    ////////////////

}
void KasiditBlueZEngine::EngineStateChangeSlot(int aState)
{
    switch(aState)
    {
    case KasiditBlueZEngine::EBtIdle:
        iLiveSocketToDisconnect = 0;
        break;
    case KasiditBlueZEngine::EBtSearching:
        break;
    case KasiditBlueZEngine::EBtSelectingPhoneToSDP:
        {
            if(iDevList.isEmpty())
            {
                QMessageBox::information(iParentWindow, tr("No nearby Bluetooth devices found"),tr("No nearby Bluetooth devices found.\r\n\r\nPlease install or start (if already installed) the mobile program on your phone and try again."));
                emit EngineStatusMessageSignal("No nearby Bluetooth devices found");
                emit EngineStateChangeSignal(EBtIdle);
            }
            else
            {
                SelectPhoneDialog w;
                int aSelIndex=-1;
                w.SetList(iDevList,&aSelIndex);
                w.exec();
                if( aSelIndex >=0 ) //selected
                {
                    qDebug("aselindex %d",aSelIndex);
                 StartSDPToSelectedDev(aSelIndex);
                }
                else //closed/cancelled
                {
                    emit EngineStateChangeSignal(EBtIdle);
                    emit EngineStatusMessageSignal("Connect Cancelled");
                }
            }
        }
        break;
    case KasiditBlueZEngine::EBtSearchingSDP:
    {




    }
        break;

    case KasiditBlueZEngine::EBtSearchingSDPDone:
    {
            if(iRFCOMMChannel<0) //not found
            {
                emit EngineStatusMessageSignal("mobile program not started");
                QMessageBox::information(iParentWindow, tr("program not started on phone"),tr("Can't find the mobile-side program running on selected mobile.\r\n\r\nPlease install/start the mobile program on your phone and try again."));
                emit EngineStateChangeSignal(EBtIdle);
            }
            else
            {
                emit EngineStatusMessageSignal("channel found");

                //start connect RFCOMM thread
                //////////////
                if(iThread && iThread->isRunning())
                 {
                iThread->wait();
                 }

                //if comes here measn thread has finished
                delete iThread;

                iThread = new CRFCOMMThread(*this);
                iThread->start();
                ////////////////

            }
    }
    break;



    case KasiditBlueZEngine::EBtConnectingRFCOMM:

        break;
    case KasiditBlueZEngine::EBtConnectionActive:

        break;
    case KasiditBlueZEngine::EBtDisconnected:
        iLiveSocketToDisconnect = 0;
        break;
    default:

        break;

    }
}

/////////////////
