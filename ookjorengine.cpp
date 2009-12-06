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

#include "ookjorengine.h"

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

const int KMaxInBufferLen = 1024*1024*2;


OokjorEngine::OokjorEngine(QWidget* aParentWindow)
{
   iParentWindow = aParentWindow;
   QObject::connect(this, SIGNAL(EngineStateChangeSignal(int)),this, SLOT (EngineStateChangeSlot(int)));
    iThread = NULL;
    iLiveSocketToDisconnect = 0;
}

OokjorEngine::~OokjorEngine()
{
    if(iLiveSocketToDisconnect!=0)
        Disconnect(); //this would stop running rfcomm thread

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

void OokjorEngine::CSDPThread::run()
{
    qDebug("entered sdp run");

    emit iFather.EngineStateChangeSignal(EBtSearchingSDP);
    emit iFather.EngineStatusMessageSignal("Searching device...");
    int channel = -1;
    iFather.iRFCOMMChannel = channel; //set to invalid

            //adjusted from http://people.csail.mit.edu/albert/bluez-intro/x604.html

            // Ookjor mobile service id 0x2BC2B92E,0x399211DC, 0x83140800, 0x200C9A38
            uint8_t svc_uuid_int[] = {0x2B,0xC2,0xB9,0x2E,0x39,0x92,0x11,0xDC,0x83,0x14,0x08,0x00,0x20,0x0C,0x9A,0x38};

            uuid_t svc_uuid;
            int err;
            bdaddr_t target;

            qDebug("copy bdaddr");
            CopyBDADDR(iFather.iDevList[iFather.iSelectedIndex].iAddr, target.b);
            qDebug("copy bdaddr complete");

            sdp_list_t *response_list = NULL, *search_list, *attrid_list;
            sdp_session_t *session = 0;

            emit iFather.EngineStatusMessageSignal("Connect to SDP on remote");
            // connect to the SDP server running on the remote machine
            session = sdp_connect( BDADDR_ANY, &target, SDP_RETRY_IF_BUSY );

            // specify the UUID of the application we're searching for
            sdp_uuid128_create( &svc_uuid, &svc_uuid_int );
            search_list = sdp_list_append( NULL, &svc_uuid );

            // specify that we want a list of all the matching applications' attributes
            uint32_t range = 0x0000ffff;
            attrid_list = sdp_list_append( NULL, &range );

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

    iFather.iRFCOMMChannel = channel;
    emit iFather.EngineStateChangeSignal(EBtSearchingSDPDone);
}

void OokjorEngine::OnNewJpgData(QByteArray& ba)
{
    iMutex.lock();
    iNewJpgBuffer = ba;
    iMutex.unlock();    

    emit GotNewJpgSignal();
}

//adapted from http://people.csail.mit.edu/albert/bluez-intro/x502.html
void OokjorEngine::CRFCOMMThread::run()
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
    addr.rc_channel = (uint8_t) 1;
    //str2ba( dest, &addr.rc_bdaddr ); already copied addr above

    // connect to server
    socklen_t addrlen = sizeof(addr);

    emit iFather.EngineStatusMessageSignal("Connecting to Bluetooth device");
    emit iFather.EngineStateChangeSignal(EBtConnectingRFCOMM);

    status = ::connect(s, (__const struct sockaddr *)&addr,addrlen );

    //Read
    if( status == 0 ) {

        qDebug("wait 2 sec before read to make sure mobile accepted connection and fully opened socket"); //otherwise strange blocking read and mobile nondisconnecting issues are observed
        sleep(2);
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



    emit iFather.EngineStatusMessageSignal("Connected, reading first frame...");
    qDebug("presignal state change 0");
    emit iFather.EngineStateChangeSignal(EBtConnectionActive);
    qDebug("postsignal state change 0");

        int bytes_read;
        //uint32_t count=1;
        //uint32_t totalb=0;
        //char progress;

        int jpgstartindex,jpgendindex;


        while(true)
        {
            //memset(buf,0,KReadBuffSize);
            //qDebug("Reading...");
            bytes_read = ::read(s, buf, KReadBuffSize);
            //qDebug("Read %d bytes",bytes_read);
            if( bytes_read > 0 )
            {
                /*QString str;
                switch(count%4)
                {
                case 0: progress = '/';break;
                case 1: progress = '-';break;
                case 2: progress = '\\';break;
                case 3: progress = '|';break;
                }
                totalb += bytes_read;*/

                if(jpgbuff.length() > KMaxInBufferLen)
                    jpgbuff.clear();

                jpgbuff.append((const char*)buf,bytes_read);

                //find and report all jpgs found

                while(true)
                {
                    //qDebug("finding jpg in buffer");

                    jpgstartindex = jpgbuff.indexOf(qKJpgHeader);
                    jpgendindex = jpgbuff.indexOf(qKJpgFooter);

                    if(jpgstartindex>=0 && jpgendindex > jpgstartindex)
                    {
                        QByteArray ajpg = jpgbuff.mid(jpgstartindex,jpgendindex-jpgstartindex);
                        iFather.OnNewJpgData(ajpg);
                        jpgbuff.remove(0,jpgendindex+1);
                        //qDebug("found jpeg");
                    }
                    else
                    {
                        //qDebug("no more jpeg");
                        break;
                    }

                }

                //////////




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

void OokjorEngine::Disconnect()
{
    qDebug("preparing to close socket handle %d",iLiveSocketToDisconnect);
    close(iLiveSocketToDisconnect); //thise would cause the CRFCOMMThread to quit as it's waiting on read
    qDebug("closed socket");
    iLiveSocketToDisconnect = 0;
}

void OokjorEngine::EngineStateChangeSlot(int aState)
{    
    switch(aState)
    {
    case OokjorEngine::EBtIdle:      
        iLiveSocketToDisconnect = 0;
        break;
    case OokjorEngine::EBtSearching:        
        break;
    case OokjorEngine::EBtSelectingPhoneToSDP:
        {
            if(iDevList.isEmpty())
            {
                QMessageBox::information(iParentWindow, tr("Ookjor: No nearby Bluetooth devices found"),tr("No nearby Bluetooth devices found.\r\n\r\nPlease install/start the Ookjor mobile program on your phone and try again."));
                emit EngineStateChangeSignal(EBtIdle);
                emit EngineStatusMessageSignal("No nearby Bluetooth devices found");
            }
            else
            {
                SelectPhoneDialog w;
                int aSelIndex=-1;
                w.SetList(iDevList,&aSelIndex);
                w.exec();
                if( aSelIndex >=0 ) //selected
                {
                    iSelectedIndex = aSelIndex;
                    QString str;
                    str = iDevList[aSelIndex].iName;
                    str += " selected, preparing to search for service...";
                    emit EngineStatusMessageSignal(str);


                    //////////////// start sdp search thread
                    if(iThread && iThread->isRunning())
                     {
                        qDebug("Warning: waiting on scan thread? actually no thread should be active now...");
                        iThread->wait();
                        qDebug("wait thread ended");
                     }

                    //if comes here measn thread has finished
                    qDebug("deleting iThread");

                    delete iThread;
                    iThread = NULL;

                    iThread = new CSDPThread(*this);
                    qDebug("created sdp thread");
                    iThread->start();
                    qDebug("started sdp thread");
                    emit EngineStateChangeSignal(EBtSearchingSDP);
                    ////////////////


                }
                else //closed/cancelled
                {                    
                    emit EngineStateChangeSignal(EBtIdle);
                    emit EngineStatusMessageSignal("Connect Cancelled");
                }
            }
        }
        break;
    case OokjorEngine::EBtSearchingSDP:
    {




    }
        break;

    case OokjorEngine::EBtSearchingSDPDone:
    {
            if(iRFCOMMChannel<0) //not found
            {
                emit EngineStatusMessageSignal("ookjor not opened on mobile");
                QMessageBox::information(iParentWindow, tr("Ookjor not started on phone"),tr("Can't find Ookjor running on selected mobile.\r\n\r\nPlease install/start the Ookjor mobile program on your phone and try again."));
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



    case OokjorEngine::EBtConnectingRFCOMM:

        break;
    case OokjorEngine::EBtConnectionActive:

        break;
    case OokjorEngine::EBtDisconnected:
        iLiveSocketToDisconnect = 0;
        break;
    default:

        break;

    }   
}

/////////////////
