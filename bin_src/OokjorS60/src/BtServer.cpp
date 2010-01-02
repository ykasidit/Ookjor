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
#include "BtServer.h"
#include <bt_sock.h>
#include <s32mem.h>
#include <centralrepository.h> // for CRepository
#include <btserversdkcrkeys.h> // for KCRUidBluetoothPowerState, KBTPowerState
#include <btnotifierapi.h> // for KPowerModeSettingNotifierUid
#include <aknnotewrappers.h>

_LIT(KServerTransportName,"RFCOMM");

CBtServer::CBtServer(MBtServerCaller& aCaller,const TDesC& aSName,const TDesC&  aSDesc, TUUID& aUUID):iBtServerCaller(aCaller),CActive(CActive::EPriorityHigh)
{

	iSName = aSName;
	iSDesc = aSDesc;
	iUUID = aUUID;

	CActiveScheduler::Add(this);
};

CBtServer::~CBtServer()
{
	StopServer();
};

void CBtServer::StartServerL()
{
	StopServer();

		//bt on or off?
		CRepository* crep = CRepository::NewLC(KCRUidBluetoothPowerState);
	    TInt value = 0;
	    User::LeaveIfError(crep->Get(KBTPowerState, value));


	    if(value==0) //bt off
	    {
	    	 RNotifier notifier;
	    	    User::LeaveIfError( notifier.Connect() );
	    	    TPckgBuf<TBool> dummy(ETrue);
	    	    TPckgBuf<TBool> reply(EFalse);
	    	    TRequestStatus status;
	    	    notifier.StartNotifierAndGetResponse(status,
	    	        KPowerModeSettingNotifierUid, dummy, reply);
	    	    User::WaitForRequest(status);
	    	    notifier.CancelNotifier(KPowerModeSettingNotifierUid);
	    	    notifier.Close();

	    	    //if user turns on now, wait for it to be fully on
	    	    User::After(2000000);
	    }



	    //if user not allowed to turn bt off
	    User::LeaveIfError(crep->Get(KBTPowerState, value));
	    if(value==0)
	    {
	    	 _LIT(msg,"Ookjor needs Bluetooth to work, exiting...");
	    		        	CAknInformationNote* informationNote = new (ELeave) CAknInformationNote(ETrue);
	    		        	informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
	    		        	informationNote->ExecuteLD(msg);
	    		        	User::Exit(0);

	    }


	    CleanupStack::PopAndDestroy(crep);


	    User::LeaveIfError(iSocketServer.Connect());
	    User::LeaveIfError(iListeningSocket.Open(iSocketServer, KServerTransportName));

		User::LeaveIfError(iListeningSocket.GetOpt(KRFCOMMGetAvailableServerChannel, KSolBtRFCOMM, iServiceChannel));


		listeningAddress.SetPort(iServiceChannel);

		User::LeaveIfError(iListeningSocket.Bind(listeningAddress));
		User::LeaveIfError(iListeningSocket.Listen(1));

	    iAdvertiser = new CBtAdvertiser(iSName,iSDesc,iUUID);
	    iAdvertiser->StartAdvertiserL(iServiceChannel);
	    iAdvertiser->UpdateAvailabilityL(ETrue);

		AcceptConnectionsL();
}

void CBtServer::StopServer()
{
	Cancel();

	if(iAdvertiser)
		iAdvertiser->UpdateAvailabilityL(EFalse);

	if (iAcceptedSocket.SubSessionHandle() != 0)
		iAcceptedSocket.Close();

	if (iListeningSocket.SubSessionHandle() != 0)
		iListeningSocket.Close();

	if (iSocketServer.Handle() != 0)
		iSocketServer.Close();

	delete iAdvertiser;
	iAdvertiser = NULL;
	iState = EIdle;
}

void CBtServer::AcceptConnectionsL()
{
	iState = EWaitingComputer;
	User::LeaveIfError(iAcceptedSocket.Open(iSocketServer));
	iListeningSocket.Accept(iAcceptedSocket, iStatus);
	SetActive();

	iBtServerCaller.OnBtServerStateChanged(iState,0,_L("Waiting for incoming connections"));
}

void CBtServer::DoCancel()
	{
	switch(iState)
		{
			case EWaitingComputer:
			iListeningSocket.CancelAccept();
			break;

			case ESendingData:
			iAcceptedSocket.CancelSend();
			break;

			default:
				User::Panic(_L("Invalid DoCancel state"),iState);
			break;
		}
	}


void CBtServer::RunL()
	{
			switch (iState)
			{

			case EWaitingComputer:
				{
					iState = EConnected;
					if (iStatus.Int() == KErrNone)
					{
						/*if(iAdvertiser)
							iAdvertiser->UpdateAvailabilityL(EFalse); //already engaged*/

						iBtServerCaller.OnBtServerStateChanged(iState,iStatus.Int(),_L("Connected"));
					}
					else
					{
						StopServer();
						iBtServerCaller.OnBtServerStateChanged(iState,iStatus.Int(),_L("AcceptFailed"));
					}
				}
			break;

			case ESendingData:
			{
				iState = EDataSent;
				if (iStatus.Int() == KErrNone)
					iBtServerCaller.OnBtServerStateChanged(iState,iStatus.Int(),_L("sent"));
				else
				{
					StopServer();
					iBtServerCaller.OnBtServerStateChanged(iState,iStatus.Int(),_L("send failed - disconnected"));
					TRAPD(err,
					StartServerL();
					);
				}
			}
			break;


			default:
				User::Panic(_L("Invalid RunL state"),iState);
			break;
			};
	}


void CBtServer::SendL(const TDesC8& aMessage)
{
	if(iState < EConnected)
		User::Leave(KErrNotReady);

		iState = ESendingData;
		iAcceptedSocket.Write(aMessage, iStatus);
		SetActive();
}


