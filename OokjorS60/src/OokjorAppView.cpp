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


#include <eikenv.h>
#include <coemain.h>
#include <Ookjor.rsg>
#include <eikmenup.h>
#include "OokjorAppui.h"

#include "OokjorApplication.h"
#include "Ookjor.hrh"
#include "OokjorContainer.h"
//#include "IncallertContainer.h"

#include "OokjorAppView.h"





COokjorAppView::COokjorAppView(COokjorAppUi *ui)
    {
		iId = TUid::Uid(ENavigationPaneStatusViewId);
		appui = ui;
    }

COokjorAppView::~COokjorAppView()
    {
		delete iContainer; //just in case
		delete iSSBitmap;
		delete iImageEncoder;
		delete iJPGSSBuffer;
    }



TUid COokjorAppView::Id() const
	{
	   return iId;
	}

// Standard construction sequence
COokjorAppView* COokjorAppView::NewL(COokjorAppUi* ui)
	{
	    COokjorAppView* self = COokjorAppView::NewLC(ui);
	    CleanupStack::Pop(self);
	    return self;
    }

COokjorAppView* COokjorAppView::NewLC(COokjorAppUi* ui)
    {

	    COokjorAppView* self = new (ELeave) COokjorAppView(ui);
	    CleanupStack::PushL(self);
	    self->ConstructL();
	    return self;
    }


void COokjorAppView::ConstructL()
    {
	    BaseConstructL(R_STATUS_VIEW);

	    iScreenDevice = CCoeEnv::Static()->ScreenDevice();
	    iSSBitmap = new (ELeave) CFbsBitmap;

	    iSSBitmap->Create( iScreenDevice->SizeInPixels(),iScreenDevice->DisplayMode() );

	    iJPGSSBuffer = NULL;
	    iImageEncoder = NULL;

	    _LIT(KBTServiceName, "Ookjor");
	    _LIT(KBTServiceDesc, "Ookjor");

		#define KBT_serviceID 0x2BC2B92E,0x399211DC, 0x83140800, 0x200C9A38
	    TUUID serviceUUID(KBT_serviceID);


	    iBtServer = new CBtServer(*this,KBTServiceName,KBTServiceDesc,serviceUUID);

	    iBtServer->StartServerL();

    }

TBool COokjorAppView::TakeScreenshot()
	{
		if(iScreenDevice->CopyScreenToBitmap( iSSBitmap ) == KErrNone)
		{

			delete iImageEncoder;
			delete iJPGSSBuffer;
			iImageEncoder = NULL;
			iJPGSSBuffer = NULL;

			iImageEncoder = CImageEncoder::DataNewL(iJPGSSBuffer,CImageEncoder::EOptionAlwaysThread,KImageTypeJPGUid);
			TRequestStatus status;
			iImageEncoder->Convert(&status,*iSSBitmap);
			User::WaitForRequest(status);
			if(status.Int()==KErrNone)
				return ETrue;
			else
				return EFalse;
		}
		else
		{
			return EFalse;
		}
		return EFalse;
	}


void COokjorAppView::HandleCommandL(TInt aCommand)
    {
         appui->HandleCommandL(aCommand);
    }

void COokjorAppView::DoActivateL(const TVwsViewId& /*aPrevViewId*/,
                                    TUid /*aCustomMessageId*/,
                                    const TDesC8& /*aCustomMessage*/)
    {
	    ASSERT(iContainer == NULL);
		iContainer = COokjorContainer::NewL(ClientRect());
		COokjorContainer* container = (COokjorContainer*) iContainer;
		container->SetStateL(iState);
		container->SetStatusL(iStatus);
		container->SetHintL(iHint);
		container->SizeChanged();
		container->DrawNow();
		AppUi()->AddToStackL(iContainer);
    }

void COokjorAppView::DoDeactivate()
    {
       if (iContainer)
        {

	    AppUi()->RemoveFromStack(iContainer);
        delete iContainer;
        iContainer = NULL;
        }
    }

void COokjorAppView::OnBtServerStateChanged(CBtServer::TState aState, TInt err, const TDesC& aDesc)
{

	COokjorContainer* container = (COokjorContainer*) iContainer;


	switch(aState)
	{
		case CBtServer::EIdle:
		{
			iState = _L("Starting up...");
			iStatus = _L("Please wait...");
			iHint = _L("Please wait...");
			if(container)
			{
			container->SetStateL(iState);
			container->SetStatusL(iStatus);
			container->SetHintL(iHint);
			container->SizeChanged();
			container->DrawNow();
			}
		}
			break;
		case CBtServer::EWaitingComputer:
		{
			iState = _L("Press \"Connect\" on Computer");
			iStatus = _L("Ookjor on computer");
			iHint = _L("Get from www.ClearEvo.com");

			if(container)
			{
			container->SetStateL(iState);
			container->SetStatusL(iStatus);
			container->SetHintL(iHint);
			container->SizeChanged();
			container->DrawNow();
			}
		}
			break;

		case CBtServer::ESendingData:
		{
		}
		break;

		/////////////////////
		case CBtServer::EConnected:
		{
			iState = _L("Connected");
			iStatus = _L("Streaming screen to computer...");
			iHint = _L("Ookjor by www.ClearEvo.com");
			if(container)
			{
			container->SetStateL(iState);
			container->SetStatusL(iStatus);
			container->SetHintL(iHint);
			container->SizeChanged();
			container->DrawNow();
			}
		}//same handling as EDataSent so DONT BREAK - follow through
		case CBtServer::EDataSent:
		{
			//update frame count

			//take ss, wait for callback, send in callback
			if(err == KErrNone && TakeScreenshot())
			{
				iBtServer->SendL(*iJPGSSBuffer);
			}
			else
				iBtServer->StopServer();
		}
		break;
		/////////////////////

	}


}

