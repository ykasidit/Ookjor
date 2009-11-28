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


#include <avkon.hrh>
#include <aknnotewrappers.h>
#include <e32base.h>
#include <aknnotedialog.h>
//#include <apgtask.h>
#include <aknglobalmsgquery.h>
#include "OokjorAppui.h"
#include "OokjorAppView.h"
#include "OokjorSettingsView.h"
#include <Ookjor.rsg>
#include "Ookjor.hrh"
#include "OokjorApplication.h"
#include "HelpAppView.h"

#include <aknmessagequerydialog.h>
#include "SettingsListSettings.h"

#include "AboutAppView.h"
#include <s32file.h>

void COokjorAppUi::ConstructL()
    {

#ifdef EKA2
  BaseConstructL(EAknEnableSkin);
#else
  //BaseConstructL(KEnableSkinFlag | KLayoutAwareFlag);
  BaseConstructL(0x08 | 0x1000);
#endif

	CEikStatusPane* sp = StatusPane();
iNaviPane = (CAknNavigationControlContainer *)sp->ControlL(TUid::Uid(EEikStatusPaneUidNavi));
sp->SetDimmed(ETrue);

	iDecoratedTabGroup = iNaviPane->ResourceDecorator();

if (iDecoratedTabGroup)
   {
   iTabGroup = (CAknTabGroup*) iDecoratedTabGroup->DecoratedControl();
   }

if (iTabGroup)
   {
	iTabGroup->SetActiveTabById(ENavigationPaneStatusViewId);
   }

  //we dont use autostart in this app
	 iAppView = COokjorAppView::NewL(this);
 iSettingsView = COokjorSettingsView::NewL(this);

 iHelpAppView = CHelpAppView::NewL( ENavigationPaneHelpViewId);
 iAboutAppView = CAboutAppView::NewL( ENavigationPaneAboutViewId);

	_LIT(KIni,"Ookjor.ini");
 TFileName fname(KIni);
	#ifndef __WINS__
 CompleteWithPrivatePathL(fname);
	#endif
 TSettingsListSettings settings;

 TRAPD(err,
 RFs fs = iCoeEnv->FsSession();

 User::LeaveIfError(fs.Connect());
 CleanupClosePushL(fs);
 RFileReadStream rfrs;
 if(rfrs.Open(fs,fname,EFileRead) == KErrNone)
 {
	    CleanupClosePushL(rfrs);
 	settings.InternalizeL(rfrs);
 	CleanupStack::PopAndDestroy();
	}
	CleanupStack::PopAndDestroy();
	);



 AddViewL(iAboutAppView);
 AddViewL(iSettingsView); // transfer ownership to base class
 AddViewL(iHelpAppView);
 AddViewL(iAppView);    // transfer ownership to base class
 SetDefaultViewL(*iAppView);







    }

COokjorAppUi::COokjorAppUi()
    {
	//iStartTime.HomeTime();
    }

COokjorAppUi::~COokjorAppUi()
    {
     delete iDecoratedTabGroup;
     //delete iExitTimer;

    }


 TKeyResponse COokjorAppUi::HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
 {

 	 TKeyResponse ret = iTabGroup->OfferKeyEventL(aKeyEvent,aType);

 	 if(ret == EKeyWasConsumed)
 	 	ActivateLocalViewL(TUid::Uid(iTabGroup->ActiveTabId()));

 	 return ret;
 }

 void COokjorAppUi::HandleCommandL(TInt aCommand)
    {

	 switch(aCommand)
	 {
	 case EAknSoftkeyExit:
	 //flow through to next case
	 case EEikCmdExit:
	 {

			Exit();
		    return;
	 }
	 default:
	 break;
	 }

    if(CAknViewAppUi::iView == iAppView)
    {
	    switch(aCommand)
	        {


	        case EOokjorSettingsCommand:
	        	{
		iTabGroup->SetActiveTabById(ENavigationPaneSettingsViewId);
	    ActivateLocalViewL(TUid::Uid(iTabGroup->ActiveTabId()));
	        	};
	        	break;

	        	case EAknSoftkeyDone:
		        {

				 _LIT(msg,"Switching to Background");
	        	CAknInformationNote* informationNote = new (ELeave) CAknInformationNote(ETrue);
	        	informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
	        	informationNote->ExecuteLD(msg);

				 TApaTask task(iEikonEnv->WsSession( ));
				 task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
				 task.SendToBackground();
		        }
	        break;


	        case EOokjorHelpCommand:
	        	{

				iTabGroup->SetActiveTabById(ENavigationPaneHelpViewId);
				ActivateLocalViewL(TUid::Uid(iTabGroup->ActiveTabId()));


	        	};
	        	break;
	        case EOokjorAboutCommand:
	        	{

	        	iTabGroup->SetActiveTabById(ENavigationPaneAboutViewId);
				ActivateLocalViewL(TUid::Uid(iTabGroup->ActiveTabId()));

	        	};
	        	break;

	        default:
	            break;
	        }
    	}
    	else
    	if(CAknViewAppUi::iView == iHelpAppView || CAknViewAppUi::iView == iAboutAppView)
    	{
			iTabGroup->SetActiveTabById(ENavigationPaneStatusViewId);
			ActivateLocalViewL(TUid::Uid(iTabGroup->ActiveTabId()));
    	}

    }

void COokjorAppUi::HandleWsEventL(const TWsEvent &aEvent, CCoeControl *aDestination)
	{
		switch (aEvent.Type())
		{
			//case KAknUidValueEndKeyCloseEvent:  0x101F87F0
			case 0x101F87F0:
			{
				TApaTask task(iEikonEnv->WsSession( ));
				task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
				task.SendToBackground();
			}
			break;

			default:
			{
			CAknAppUi::HandleWsEventL(aEvent, aDestination);
			}
		}
	}

