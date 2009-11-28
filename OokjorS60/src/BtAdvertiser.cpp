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

#include "BtAdvertiser.h"

CBtAdvertiser::CBtAdvertiser(const TDesC& aSName,const TDesC&  aSDesc, TUUID& aUUID)
{
	iSName = aSName;
	iSDesc = aSDesc;
	iUUID = aUUID;
}

CBtAdvertiser::~CBtAdvertiser()
{
	TRAPD(err,StopAdvertiserL(););

	iSdpDB.Close();
	iSdp.Close();
}

void CBtAdvertiser::StartAdvertiserL(TInt aChannel)
    {
    // Open sdp session
    User::LeaveIfError(iSdp.Connect());
    // Open sdp database session
    User::LeaveIfError(iSdpDB.Open(iSdp));

    // Create a record of the correct service class

    iSdpDB.CreateServiceRecordL(iUUID, iRecord);

    // Add a protocol to the record
    CSdpAttrValueDES* protocolDescriptorList = CSdpAttrValueDES::NewDESL(NULL);
    CleanupStack::PushL(protocolDescriptorList);

    TBuf8<1> channel;
    channel.Append((TChar)aChannel);

    // Create protocol list for our service
    protocolDescriptorList
    ->StartListL()   //  list of protocols required for this method
        ->BuildDESL()
        ->StartListL()
            ->BuildUUIDL(KL2CAP)
        ->EndListL()

        ->BuildDESL()
        ->StartListL()
            ->BuildUUIDL(KRFCOMM)
            ->BuildUintL(channel)
        ->EndListL()
    ->EndListL();

    // Set protocol list to the record
    iSdpDB.UpdateAttributeL(iRecord, KSdpAttrIdProtocolDescriptorList,
        *protocolDescriptorList);
    CleanupStack::PopAndDestroy(protocolDescriptorList);

    // Add a name to the record
    iSdpDB.UpdateAttributeL(iRecord,
                                KSdpAttrIdBasePrimaryLanguage +
                                    KSdpAttrIdOffsetServiceName,
                                    iSName);

    // Add a description to the record
    iSdpDB.UpdateAttributeL(iRecord,
                                KSdpAttrIdBasePrimaryLanguage +
                                    KSdpAttrIdOffsetServiceDescription,
                                    iSDesc);

    // Set service available
    UpdateAvailabilityL(ETrue);
    }


void CBtAdvertiser::UpdateAvailabilityL(TBool aAvailable)
    {
    TInt state = aAvailable ? 0xFF : 0x00;

    // Set availability
    iSdpDB.UpdateAttributeL(iRecord, KSdpAttrIdServiceAvailability, state);

    // Mark record changed
    iSdpDB.UpdateAttributeL(iRecord, KSdpAttrIdServiceRecordState,
        ++iRecordState);
    }


void CBtAdvertiser::StopAdvertiserL()
    {
    if ( iRecord!=NULL )
        {
        // Delete record from service discovery database
        iSdpDB.DeleteRecordL(iRecord);

        // Close sdp and sdp db sessions
        iSdpDB.Close();
        iSdp.Close();
        iRecord=NULL;
        }
    }


