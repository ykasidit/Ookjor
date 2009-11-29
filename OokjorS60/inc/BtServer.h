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

#ifndef BTSERVER_H_
#define BTSERVER_H_

#include <es_sock.h>

#include "BtAdvertiser.h"
// User defined include files
#include <btmanclient.h>

class MBtServerCaller;

class CBtServer: public CActive
{
	public:

	enum TState
	{
		EIdle, //stopped
		EWaitingComputer, //started
		EConnected,
		ESendingData,
		EDataSent
	};

	CBtServer(MBtServerCaller& aCaller,const TDesC& aSName,const TDesC&  aSDesc, TUUID& aUUID);
	~CBtServer();

	void StartServerL(); //call manually
	void StopServer(); //automatically called by dtor too
	void SendL(const TDesC8& aMessage);

	public:
		//CActive
	void RunL();
	void DoCancel();

protected:

	void AcceptConnectionsL();

	MBtServerCaller&								iBtServerCaller;
	CBtAdvertiser* 									iAdvertiser;
	TState iState;

	RSocketServ										iSocketServer;
	RSocket											iListeningSocket;
	RSocket											iAcceptedSocket;

	TBuf<32> iSName;
	TBuf<32> iSDesc;
	TUUID 	iUUID;
	TInt	iServiceChannel;

};

class MBtServerCaller
{
	public:
	virtual void OnBtServerStateChanged(CBtServer::TState aState, TInt err, const TDesC& aDesc)=0;
};
#endif /* BTSERVER_H_ */
