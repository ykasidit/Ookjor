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

class COokjorAppUi;
class COokjorContainer;
class CWsClient;
/*!
  @class COokjorAppView

  @discussion An instance of the Application View object for the Ookjor
  example application
  */
class COokjorAppView :  public CAknView, public MBtServerCaller
    {
public:

	////////
	void OnBtServerStateChanged(CBtServer::TState aState, TInt err, const TDesC& aDesc);
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

	protected:
	COokjorAppUi *appui;
	CCoeControl *iContainer;

	CBtServer* iBtServer;

	COokjorAppView(COokjorAppUi *ui);
	void ConstructL();
	TUid iId;

	TBool TakeScreenshot();

	CWsScreenDevice* iScreenDevice;
	CFbsBitmap* iSSBitmap;
	CImageEncoder* iImageEncoder;
	HBufC8* iJPGSSBuffer;
    };


#endif // __Ookjor_APPVIEW_H__
