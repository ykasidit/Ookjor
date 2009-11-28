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


#include "OokjorSettingsView.h"
#include "OokjorSettingsContainer.h"
#include "OokjorAppui.h"
#include <eikapp.h>
#include "SettingsListSettingItemList.h"
#include <eiklbx.h>
#include "SettingsListSettingItemList.h"
#include <Ookjor.rsg>
#include <eikenv.h>
#include <s32file.h>


COokjorSettingsContainer* COokjorSettingsContainer::NewL(const TRect& aRect,COokjorAppUi *ui)
    {
    COokjorSettingsContainer* self = COokjorSettingsContainer::NewLC(aRect,ui);
    CleanupStack::Pop(self);
    return self;
    }

COokjorSettingsContainer* COokjorSettingsContainer::NewLC(const TRect& aRect,COokjorAppUi *ui)
    {
    COokjorSettingsContainer* self = new (ELeave) COokjorSettingsContainer(ui);
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    return self;
    }

void COokjorSettingsContainer::GetIniPath(TFileName& aIniFileName)
	{

	TFileName app = CEikonEnv::Static()->EikAppUi()->Application()->AppFullName();
	TParsePtr parse(app);
	#ifndef __WINS__
		aIniFileName.Copy(parse.DriveAndPath());
	#endif
	aIniFileName.Append(parse.Name());
	_LIT(KIni, ".ini");
	aIniFileName.Append(KIni);
	}

void COokjorSettingsContainer::ConstructL(const TRect& aRect)
    {


    CreateWindowL();

    //settingsFileName = iCoeEnv->AllocReadResourceL(R_SETTINGSFILE_STR);

    TFileName fname;
    GetIniPath(fname);

    RFs fs = iCoeEnv->FsSession();
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    RFileReadStream rfrs;
    if(rfrs.Open(fs,fname,EFileRead) == KErrNone)
    {
		CleanupClosePushL(rfrs);
    	iSettings.InternalizeL(rfrs);
    	CleanupStack::PopAndDestroy();
   	}
	CleanupStack::PopAndDestroy();
	SetRect(aRect);

    iSettingItemList = new (ELeave) CSettingsListSettingItemList (iSettings);
//    iSettingItemList->SetMopParent(this);
    iSettingItemList->ConstructFromResourceL(R_SETTINGSLIST_SETTING_ITEM_LIST);
  //  iSettingItemList->ListBox()->CreateScrollBarFrameL(ETrue);
//    iSettingItemList->ListBox()->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff,CEikScrollBarFrame::EOn);

//	iSettingItemList->ListBox()->UpdateScrollBarsL();

	ActivateL();

    }

TInt COokjorSettingsContainer::CountComponentControls() const
    {
    return 1;
    //return 0;
    }

void COokjorSettingsContainer::Draw(const TRect& aRect) const
    {
    // Get the standard graphics context
 	CWindowGc& gc = SystemGc();

    // Gets the control's extent

    gc.Clear(aRect);
//    iSettingItemList->DrawDeferred();

/*    const CFont* font = CEikonEnv::Static()->TitleFont();
	    gc.UseFont(font);
	    gc.SetPenColor(0x009911);
	    gc.SetPenStyle(CGraphicsContext::ESolidPen);
	    TPoint p(50,50);
	    _LIT(msg,"ALLAHUAKBAR!");
	    gc.DrawText(msg,p);
	    gc.DiscardFont();*/

   }

CCoeControl* COokjorSettingsContainer::ComponentControl(TInt aIndex) const
    {
     switch (aIndex)
		{
		case 0:
			return iSettingItemList;
		default:
			return NULL;
		}

    }

    COokjorSettingsContainer::COokjorSettingsContainer(COokjorAppUi*)
    {
//    	appui = ui;
//   	    this->SetMopParent(appui->iSettingsView);
    }
    COokjorSettingsContainer::~COokjorSettingsContainer()
    {
    	//delete settingsFileName;
    	delete iSettingItemList;
    	iSettingItemList = NULL;
    }
    TKeyResponse COokjorSettingsContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
	{
		TKeyResponse ret = EKeyWasNotConsumed;



		if(iSettingItemList)
			if((aKeyEvent.iCode!= EKeyLeftArrow && aKeyEvent.iCode!= EKeyRightArrow))
				ret = iSettingItemList->OfferKeyEventL(aKeyEvent, aType);



		return ret;
	}

	void COokjorSettingsContainer::ChangeSelectedItemL()
	{
	if (iSettingItemList)
		iSettingItemList->ChangeSelectedItemL();
	}
	void COokjorSettingsContainer::SaveSettingsL()
	{
        	TFileName fname;

    		GetIniPath(fname);
    	    TChar drivechar = fname[0];
    		RFs fs = iCoeEnv->FsSession();
        	User::LeaveIfError(fs.Connect());
        	CleanupClosePushL(fs);
        	TInt dint;
        	fs.CharToDrive(drivechar,dint);
        	TVolumeInfo vol;
        	fs.Volume(vol,dint);

        	if(vol.iFree<5) //save file is 5 bytes
					{
						fs.Close();
						User::Leave(KErrDiskFull);
					}

        	RFileWriteStream rfws;
        	TInt err=KErrNone;
        	err = rfws.Create(fs,fname,EFileWrite);
    		if(err == KErrAlreadyExists)
    			{
    				err=KErrNone;
    				err=rfws.Open(fs,fname,EFileWrite);
    			}

    		if(err==KErrNone)
    			{
    				CleanupClosePushL(rfws);
    				iSettings.ExternalizeL(rfws);
    				CleanupStack::PopAndDestroy();
    			}

    		CleanupStack::PopAndDestroy();

	}
