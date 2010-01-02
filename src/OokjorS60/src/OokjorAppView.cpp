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

/*
 *
 * NOTE: the autofocus, capture related code are just added to plan for future dev, THEY ARE NOT USED NOW
 * */


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

		iBtState = CBtServer::EIdle; //so CleanupCamera() wont try to update screen or icontainers
		CleanupCamera();

		delete iContainer; //just in case
		delete iSSBitmap;
		delete iImageEncoder;
		delete iJPGSSBuffer;
		delete iJPGCamBuffer;

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

void COokjorAppView::UpdateStatus(const TDesC& status)
{
		iStatus = status;
		COokjorContainer* container = (COokjorContainer*) iContainer;
		if(container)
		{
		container->SetStatusL(iStatus);
		container->SizeChanged();
		container->DrawNow();
		}
}

void COokjorAppView::StartCamera()
	{

		CleanupCamera();

	 TRAPD(err,
		    iCamera = CCamera::NewL(*this,0);
			 iCamera->CameraInfo(iInfo);
			 TRAPD(afErr, iAutoFocus = CCamAutoFocus::NewL( iCamera ));
		    );

		    if(!iCamera)
		    {
		    TBuf<32> buf;
		    buf.Format(_L("Can't start camera: error %d"),err);
		   	        	CAknInformationNote* informationNote = new (ELeave) CAknInformationNote(ETrue);
		   	        	informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
		   	        	informationNote->ExecuteLD(buf);
		    }
		    else
		    {
		    	if(iBtState > CBtServer::EConnected) //if already connected but in background mode, start camera loop too
		    	{
		    		UpdateStatus(_L("Resuming Camera Mode"));
		    		iCamera->Reserve();
		    	}
		    }
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

void COokjorAppView::CleanupCamera()
{
	if(iCamera)
	{
		iCamera->StopViewFinder();
		iCamera->PowerOff();

		if(iAutoFocus)
		{
		// bring AF subsystem to idle
		TRAPD( ignore, iAutoFocus->ResetToIdleL() );
		iAutoFocus->Close();
		}

		iCamera->Release();
	}
	delete iCamera;
	iCamera = NULL;
	delete iJPGCamBuffer;
	iJPGCamBuffer = NULL;

	delete iAutoFocus;
	iAutoFocus = NULL;

	if(iBtState > CBtServer::EConnected)
	{
		UpdateStatus(_L("Streaming mobile screen"));
	}
}

void COokjorAppView::ReserveComplete(TInt err)
{

	if(err == KErrNone)
	{
		UpdateStatus(_L("Starting Camera"));
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

CCamera::TFormat COokjorAppView::ImageFormatMax() const
    {
    if ( iInfo.iImageFormatsSupported & CCamera::EFormatFbsBitmapColor16M )
        {
        return CCamera::EFormatFbsBitmapColor16M;
        }
    else if ( iInfo.iImageFormatsSupported & CCamera::EFormatFbsBitmapColor64K)
        {
        return CCamera::EFormatFbsBitmapColor64K;
        }
    else
        {
        return CCamera::EFormatFbsBitmapColor4K;
        }
    }



////////auto focus
void COokjorAppView::InitComplete( TInt aError )
{

}

void COokjorAppView::OptimisedFocusComplete( TInt aError )
{

}
///////////////

void COokjorAppView::PowerOnComplete(TInt err)
{

	if(err == KErrNone)
	{
		if( iAutoFocus )
		            {
						TRAPD( afErr, iAutoFocus->InitL( *this ) );
						if(afErr!=KErrNone)
						{
							delete iAutoFocus;
							iAutoFocus = NULL;
						}
		            }

		UpdateStatus(_L("Streaming camera view"));

		//see http://www.forum.nokia.com/info/sw.nokia.com/id/a1440080-11f2-4462-bc44-9c88a5a11482/S60_Platform_Camera_Example_with_AutoFocus_Support_v2_1_en.zip.html

		const TInt KImageSizeIndex = 1;

		 iCamera->SetExposureL();

//		    TRAPD(ignore, iCamera->SetDigitalZoomFactorL( iZoomFactor ));


		    if ( iInfo.iOptionsSupported & TCameraInfo::EViewFinderBitmapsSupported )
		        {
		        if ( iInfo.iOptionsSupported & TCameraInfo::EImageCaptureSupported)
		            {
		            iFormat = CCamera::EFormatExif;
		            TRAPD(exifErr, iCamera->PrepareImageCaptureL(iFormat, KImageSizeIndex));
		            if(exifErr) // capturing in EXIF format not supported,
		                        // fall back to bitmap format
		                {
		                iFormat = ImageFormatMax();
		                iCamera->PrepareImageCaptureL(iFormat, 1);
		                }
		            else    // notify controller that we're using EXIF capture mode
		                {
		                iCamera->EnumerateCaptureSizes( iCaptureSize, KImageSizeIndex,
		                                               iFormat );
		                }


		            }
		        }

		    TSize sz = ClientRect().Size();
		    iCamera->StartViewFinderBitmapsL( sz );

		    //iCamera->CaptureImage();

		//TSize sz(640,480);
		//iCamera->StartViewFinderBitmapsL(sz);
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
void COokjorAppView::ImageReady(CFbsBitmap *aBitmap,HBufC8 *aData,TInt aError)
{

	    if ( aError == KErrNone )
	        {
	        if( iFormat == CCamera::EFormatExif )
	            {
	            if ( iJPGCamBuffer )
	                {
	                delete iJPGCamBuffer;
	                iJPGCamBuffer = 0;
	                }

					if(aData)
						iJPGCamBuffer = aData->Alloc();
	            }
	        else
	            {

	        	if(aBitmap)
	        	{
					delete iImageEncoder;
					delete iJPGCamBuffer;
					iImageEncoder = NULL;
					iJPGCamBuffer = NULL;



					TRAPD(err,
					iImageEncoder = CImageEncoder::DataNewL(iJPGCamBuffer,CImageEncoder::EOptionAlwaysThread,KImageTypeJPGUid);
					TRequestStatus status;
					iImageEncoder->Convert(&status,*aBitmap);
					User::WaitForRequest(status);
					);
	        	}

	            }
	        }

} //Transfers the current image from the camera to the client.


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


	iBtState = aState;
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
				iCamera->Reserve(); //starts the whole camera loop
			}
			else
			{

			 	CAknInformationNote* informationNote = new (ELeave) CAknInformationNote(ETrue);
				   	        	informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
				   	        	informationNote->ExecuteLD(_L("No camera, using Screen Export mode"));
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

						iJPGSSBuffer = iJPGCamBuffer->Alloc();
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

