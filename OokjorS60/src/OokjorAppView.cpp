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
#include <aknnotewrappers.h>
#include "OokjorAppView.h"



COokjorAppView* COokjorAppView::curInstance = NULL;

COokjorAppView::COokjorAppView(COokjorAppUi *ui)
    {
		iId = TUid::Uid(ENavigationPaneStatusViewId);
		appui = ui;
		curInstance = this;

    }

COokjorAppView::~COokjorAppView()
    {
		curInstance = NULL;

		delete iContainer; //just in case
		delete iSSBitmap;
		delete iImageEncoder;
		delete iJPGSSBuffer;
		delete iJPGCamBuffer;

		CleanupCamera();
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

	    TRAPD(err,
	    iCamera = CCamera::NewL(*this,0);
	    );

	    if(!iCamera)
	    {
	    TBuf<32> buf;
	    buf.Format(_L("camera init err %d"),err);
	   	        	CAknInformationNote* informationNote = new (ELeave) CAknInformationNote(ETrue);
	   	        	informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
	   	        	informationNote->ExecuteLD(buf);
	    }


    }

void COokjorAppView::CleanupCamera()
{
	if(iCamera)
	{
		iCamera->PowerOff();
		iCamera->Release();
	}
	delete iCamera;
	iCamera = NULL;
	delete iJPGCamBuffer;
	iJPGCamBuffer = NULL;
}

void COokjorAppView::ReserveComplete(TInt err)
{

	if(err == KErrNone)
	{

		 iCamera->PowerOn();
	}
	else
	{
		TBuf<32> buf;
		buf.Format(_L("camera reserve err %d"),err);
		CAknInformationNote* informationNote = new (ELeave) CAknInformationNote(ETrue);
		informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
		informationNote->ExecuteLD(buf);
		CleanupCamera();
	}
}

void COokjorAppView::PowerOnComplete(TInt err)
{

	if(err == KErrNone)
	{

		TSize sz(640,480);
		iCamera->StartViewFinderBitmapsL(sz);
	}
	else
	{
		TBuf<32> buf;
		buf.Format(_L("camera power on err %d"),err);
		CAknInformationNote* informationNote = new (ELeave) CAknInformationNote(ETrue);
		informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
		informationNote->ExecuteLD(buf);
		CleanupCamera();
	}
}






 void COokjorAppView::ViewFinderFrameReady(CFbsBitmap &aFrame)
 {



					delete iImageEncoder;
					delete iJPGCamBuffer;
					iImageEncoder = NULL;
					iJPGCamBuffer = NULL;



					TRAPD(err,

					iImageEncoder = CImageEncoder::DataNewL(iJPGCamBuffer,CImageEncoder::EOptionAlwaysThread,KImageTypeJPGUid);
					TRequestStatus status;
					iImageEncoder->Convert(&status,aFrame);
					User::WaitForRequest(status);
		 );

 }

void COokjorAppView::FrameBufferReady(MFrameBuffer *,TInt){} //Passes a filled frame buffer to the client.
void COokjorAppView::ImageReady(CFbsBitmap *,HBufC8 *,TInt){} //Transfers the current image from the camera to the client.


TBool COokjorAppView::TakeScreenshot()
	{

		if(iScreenDevice->CopyScreenToBitmap( iSSBitmap ) == KErrNone)
		{


			delete iImageEncoder;
			delete iJPGSSBuffer;
			iImageEncoder = NULL;
			iJPGSSBuffer = NULL;

			TRequestStatus status;

			TRAPD(err,
			iImageEncoder = CImageEncoder::DataNewL(iJPGSSBuffer,CImageEncoder::EOptionAlwaysThread,KImageTypeJPGUid);
			iImageEncoder->Convert(&status,*iSSBitmap);
			User::WaitForRequest(status);
			);



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


			CleanupCamera();

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

			if(iCamera)
			{
				iCamera->StopViewFinder();
				iCamera->PowerOff();
				iCamera->Release();
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
			iStatus = _L("Streaming to computer...");
			iHint = _L("Ookjor by www.ClearEvo.com");
			if(container)
			{
			container->SetStateL(iState);
			container->SetStatusL(iStatus);
			container->SetHintL(iHint);
			container->SizeChanged();
			container->DrawNow();
			}
			if(iCamera)
			{
				iCamera->Reserve();
			}
			else
			{

			 	CAknInformationNote* informationNote = new (ELeave) CAknInformationNote(ETrue);
				   	        	informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
				   	        	informationNote->ExecuteLD(_L("no camera"));
			}

			TakeScreenshot(); //first frame init for cam case

		}
		//CONTINUE to same handling as EDataSent so DONT BREAK - follow through

		case CBtServer::EDataSent:
		{
			//update frame count

			//take ss, wait for callback, send in callback
			if(err == KErrNone)
			{
				if(!iCamera) //camera power on may not be ready yet, so export screen in the mean time, so user can also notice that this app can export screen too
				{
					CAknInformationNote* informationNote = new (ELeave) CAknInformationNote(ETrue);
					informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
					informationNote->ExecuteLD(_L("no cam2"));
					TakeScreenshot();

					if(iJPGSSBuffer)
										{
										TRAPD(err,
										iBtServer->SendL(*iJPGSSBuffer);
										);
										}
				}
				else
				{

					if(iJPGCamBuffer && iJPGSSBuffer)
					{
						delete iJPGSSBuffer;
						iJPGSSBuffer = NULL;

						iJPGSSBuffer = iJPGCamBuffer->AllocL();
					}

					if(iJPGSSBuffer)
					{
					TRAPD(err,
					iBtServer->SendL(*iJPGSSBuffer);
					);
					}
				}
			}
			else
				iBtServer->StopServer();
		}
		break;
		/////////////////////

	}


}

