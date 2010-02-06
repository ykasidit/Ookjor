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

#ifndef __Ookjor_APPVIEW_H__
#define __Ookjor_APPVIEW_H__

#include <aknview.h>

#include "BtServer.h"
#include <w32std.h>
#include <imageconversion.h>
#include <ecam.h>
#include <CCamAutoFocus.h>

class COokjorAppUi;
class COokjorContainer;
class CWsClient;
/*!
  @class COokjorAppView

  @discussion An instance of the Application View object for the Ookjor
  example application
  */
class COokjorAppView :  public CAknView, public MBtServerCaller, public MCameraObserver, public MCamAutoFocusObserver
    {
public:

	static COokjorAppView* curInstance;

	////////
	void OnBtServerStateChanged(CBtServer::TState aState, TInt err, const TDesC& aDesc);
	/////////

	//////////
	virtual void FrameBufferReady(MFrameBuffer *,TInt); //Passes a filled frame buffer to the client.
	virtual void ImageReady(CFbsBitmap *,HBufC8 *,TInt);//Transfers the current image from the camera to the client.
	virtual void ViewFinderFrameReady(CFbsBitmap &);//Tests whether transfer of view finder data has completed.

	virtual void ReserveComplete(TInt aError);
	virtual void PowerOnComplete(TInt aError);

	 virtual void InitComplete( TInt aError );
     virtual void OptimisedFocusComplete( TInt aError );
	/////////

	TUid Id() const;
	void HandleCommandL(TInt aCommand);
  	TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
  	static COokjorAppView* NewL(COokjorAppUi* ui);
  	static COokjorAppView* NewLC(COokjorAppUi* ui);
	void DoActivateL(const TVwsViewId& aPrevViewId,
                   TUid aCustomMessageId,
                   const TDesC8& aCustomMessage);
	void DoDeactivate();

	virtual ~COokjorAppView();

	void DialogDismissedL(TInt aButtonId);//MProgressDialogCallback

	void CleanupCamera();
	void StartCamera();
	void UpdateStatus(const TDesC& status);

	protected:
	COokjorAppUi *appui;
	CCoeControl *iContainer;

	CBtServer* iBtServer;

	COokjorAppView(COokjorAppUi *ui);
	void ConstructL();
	TUid iId;

	TBool TakeScreenshot();

	CCamera::TFormat ImageFormatMax() const;

	CWsScreenDevice* iScreenDevice;
	CFbsBitmap* iSSBitmap;
	CImageEncoder* iImageEncoder;
	HBufC8* iJPGSSBuffer;
	HBufC8* iJPGCamBuffer;

	TBuf<128> iState;
	TBuf<128> iStatus;
	TBuf<128> iHint;

	 CCamera *iCamera;
	 TCameraInfo iInfo;
	 CCamera::TFormat iFormat;
	 TSize iCaptureSize;
     TInt iCaptureFormat;
     CCamAutoFocus* iAutoFocus;


	 CBtServer::TState iBtState;// so StartCamera would know if app is in connected state or not, if connected then it would stat reserve, power on, etc.

	 void SetCaptureModeL(const TSize& aSize,
	                                            TInt aFormat,
	                                            TBool aAFSupported);


    };


#endif // __Ookjor_APPVIEW_H__
