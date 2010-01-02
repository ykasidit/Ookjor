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

//adapted from http://wiki.forum.nokia.com/index.php/CS000938_-_Advertising_Bluetooth_services

#ifndef BTADVERTISER_H_
#define BTADVERTISER_H_

#include <btsdp.h>
#include <bt_sock.h>

class CBtAdvertiser
{
	public:

	CBtAdvertiser(const TDesC& aSName,const TDesC&  aSDesc, TUUID& aUUID);
	~CBtAdvertiser();

	void StartAdvertiserL(TInt aChannel);
	void StopAdvertiserL();
	void UpdateAvailabilityL(TBool aAvailable);

	protected:
	// Service discovery protocol session
	RSdp iSdp;

	// Service discovery database (sdp)
	RSdpDatabase iSdpDB;

	// Service record
	TSdpServRecordHandle iRecord;

	// Service record state
	TInt iRecordState;

	TBuf<32> iSName;
	TBuf<32> iSDesc;
	TUUID iUUID;
};




#endif /* BTADVERTISER_H_ */
