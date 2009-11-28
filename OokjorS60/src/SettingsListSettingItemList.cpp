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

// INCLUDE FILES

// Class include
#include <aknview.h>
#include <eikfutil.h>
#include <aknnotewrappers.h>

// User include
#include "SettingsListSettingItemList.h"
#include "SettingsListSettings.h"
#include "OokjorSettingsView.h"
#include "OokjorAppui.h"

// ================= MEMBER FUNCTIONS =======================

/**
* Constructor.	Takes a copy of the reference to aDifficultyLevel
* @param aDifficultyLevel the level of difficulty which will be set
*/
CSettingsListSettingItemList::CSettingsListSettingItemList (TSettingsListSettings& aSettings)
: iSettings (aSettings)
	{
	}


/**
*
* Called by framework when the view size is changed.  Resizes the
* setting list accordingly.
*
*/
void CSettingsListSettingItemList::SizeChanged()
	{
	/*if (ListBox())
		{
		ListBox()->SetRect(Rect());
		}*/
	}

TKeyResponse CSettingsListSettingItemList::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
	{



    if (aType == EEventKey)
		{

		    if (aKeyEvent.iCode == EKeyLeftArrow  || aKeyEvent.iCode == EKeyRightArrow )
            {
					return EKeyWasNotConsumed;
			}

	        return CAknSettingItemList::OfferKeyEventL(aKeyEvent, aType);

		}


	return EKeyWasNotConsumed;



	}



/**
*
* Creates the actual setting items for the list, passing
* ownership of them to the calling class.  Each setting
* item has a piece of member data which it sets values in.
*/
CAknSettingItem* CSettingsListSettingItemList::CreateSettingItemL(TInt aIdentifier)
	{
	CAknSettingItem* settingItem = NULL;

	switch (aIdentifier)
		{

		case ESettingListAutoStartItem:
			{
#ifndef EKA2
			autoLoad = COokjorSettingsView::AutoStartEnabled();
#else
			autoLoad = 	COokjorSettingsView::AutoStartEnabled();
#endif

			settingItem = new (ELeave) CAknBinaryPopupSettingItem(aIdentifier,autoLoad );
			}
			break;

		case ESettingListCycleTimeItem:

			switch(iSettings.iCycleTime) //avoid panic that val is not valid for settingpage
			{
				case 1: break;
				case 2: break;
				case 3: break;
				case 5: break;
				case 10: break;
				case 15: break;
				case 30: break;
				default: iSettings.iCycleTime =1;
			}
			settingItem = new (ELeave) CAknEnumeratedTextPopupSettingItem(aIdentifier, iSettings.iCycleTime);
			break;
		case ESettingListStartMinuteItem:
			switch(iSettings.iStartMinute) //avoid panic that val is not valid for settingpage
			{
				case 1: break;
				case 2: break;
				case 3: break;
				case 4: break;
				case 5: break;
				case 10: break;
				case 15: break;
				case 30: break;
				default: iSettings.iStartMinute =1;
			}
			settingItem = new (ELeave) CAknEnumeratedTextPopupSettingItem(aIdentifier, iSettings.iStartMinute);
			break;
		case ESettingListPreCycleTimeItem:
			switch(iSettings.iPreCycleTime) //avoid panic that val is not valid for settingpage
			{
				case 0: break;
				case 3: break;
				case 5: break;
				case 10: break;
				case 15: break;
				case 30: break;
				default: iSettings.iPreCycleTime = 10;
			}
			settingItem = new (ELeave) CAknEnumeratedTextPopupSettingItem(aIdentifier, iSettings.iPreCycleTime);
			break;

		/*case ESettingsListPlayerNameSettingItem:
			settingItem = new (ELeave) CAknTextSettingItem(aIdentifier, iSettings.PlayerName());
			break;
		case ESettingsListHintsSettingItem:
			settingItem = new (ELeave) CAknBinaryPopupSettingItem (aIdentifier, iSettings.HintsEnabled());
			break;
*/
		}
	return settingItem; // passing ownership
	}

/**
* Causes the edit page for the currently selected setting
* item to be displayed
*/
void CSettingsListSettingItemList::ChangeSelectedItemL()
	{


		EditItemL(ListBox()->CurrentItemIndex(), ETrue);


	}

/**
* Called by the framework whenever an item is selected.
* Causes the edit page for the currently selected setting item to be displayed and stores
* any changes made.
*/
void CSettingsListSettingItemList::EditItemL (TInt aIndex, TBool aCalledFromMenu)
	{
		if(aIndex!=0)
		{
		CAknSettingItemList::EditItemL(aIndex, aCalledFromMenu);
		(*SettingItemArray())[aIndex]->StoreL();
		return;
		}




autoLoad = COokjorSettingsView::AutoStartEnabled();


#ifndef EKA2
	//mdl is for s60 2nd only

	#ifndef __WINS__
  	_LIT(path,"\\system\\Recogs\\Iclrstrt.mdl");
	#endif

   	    TFileName mdlFile(path);

		#ifndef __WINS__
   	 COokjorAppUi::CompleteWithPrivatePathL(mdlFile);
	    #endif

		if(	autoLoad)
		{
			TInt res = EikFileUtils::DeleteFile(mdlFile);
			if(res == KErrNone)
			{
				_LIT(msg,"Auto-Start Disabled.");
			    CAknConfirmationNote* informationNote = new (ELeave) CAknConfirmationNote(ETrue);
			    informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
			    CAknSettingItemList::EditItemL(aIndex, aCalledFromMenu);
				(*SettingItemArray())[aIndex]->StoreL();
			   	informationNote->ExecuteLD(msg);
			}
			else
			{
				_LIT(msg,"Failed to disable Auto-Start.");
			    CAknErrorNote* informationNote = new (ELeave) CAknErrorNote(ETrue);
			    informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
			   	informationNote->ExecuteLD(msg);
			}

		}
		else
		{




	    #ifdef __WINS__
  		_LIT(srcpath,"D:\\Symbian\\Series60_1_2_CW\\epoc32\\release\\WINSCW\\UDEB\\Z\\SYSTEM\\APPS\\Ookjor\\Iclrstrt.mdl");
  		#else
	    _LIT(srcpath,"Iclrstrt.mdl");
	    #endif

	    TFileName mdlFileSrc(srcpath);

		#ifndef __WINS__
	    COokjorAppUi::CompleteWithPrivatePathL(mdlFileSrc);
	    #endif

			TInt res = EikFileUtils::CopyFile(mdlFileSrc,mdlFile,CFileMan::ERecurse);
			if(res == KErrNone)
			{
				_LIT(msg,"Auto-Start Enabled.");
			    CAknConfirmationNote* informationNote = new (ELeave) CAknConfirmationNote(ETrue);
			    informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
			    CAknSettingItemList::EditItemL(aIndex, aCalledFromMenu);
				(*SettingItemArray())[aIndex]->StoreL();
			   	informationNote->ExecuteLD(msg);
			}
			else
			{
				_LIT(msg,"Failed to enable Auto-Start.");
			    CAknErrorNote* informationNote = new (ELeave) CAknErrorNote(ETrue);
			    informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
			   	informationNote->ExecuteLD(msg);
			}
		}

#else


		//s60 3rd our app is always started, make a file as flag to let app hide and then exit after a few seconds (it's a forum nokia Knowledge base issue that if autostarted app exits too soon then phone could report that the app is currupted)
		_LIT(KNoASFileFlag,"noautostart.dat");
			TFileName fn(KNoASFileFlag);
			COokjorAppUi::CompleteWithPrivatePathL(fn);

		if(!autoLoad)
		{

			EikFileUtils::DeleteFile(fn);


						_LIT(msg,"Auto-Start Enabled");
					    CAknConfirmationNote* informationNote = new (ELeave) CAknConfirmationNote(ETrue);
					    informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
					    CAknSettingItemList::EditItemL(aIndex, aCalledFromMenu);
						(*SettingItemArray())[aIndex]->StoreL();
					   	informationNote->ExecuteLD(msg);
		}
		else
		{
			RFs fs = iCoeEnv->FsSession();
						User::LeaveIfError(fs.Connect());
						CleanupClosePushL(fs);
						RFile f;
						User::LeaveIfError(f.Create(fs,fn, EFileWrite));
						f.Close();
						CleanupStack::PopAndDestroy();

			_LIT(msg,"Auto-Start Disabled");
								    CAknConfirmationNote* informationNote = new (ELeave) CAknConfirmationNote(ETrue);
								    informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
								    CAknSettingItemList::EditItemL(aIndex, aCalledFromMenu);
									(*SettingItemArray())[aIndex]->StoreL();
								   	informationNote->ExecuteLD(msg);
		}



#endif



	}

CSettingsListSettingItemList::~CSettingsListSettingItemList()
	{

	}

