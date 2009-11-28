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
#include <aknnotewrappers.h>
#include <eikmenup.h>
#include <stringloader.h>		// StringLoader
#include "OokjorAppui.h"
#include <aknnotewrappers.h>

#include "OokjorApplication.h"
#include "Ookjor.hrh"
#include "OokjorContainer.h"

#include "OokjorAppView.h"





COokjorAppView::COokjorAppView(COokjorAppUi *ui)
    {
		iId = TUid::Uid(ENavigationPaneStatusViewId);
		appui = ui;
    }

COokjorAppView::~COokjorAppView()
    {
		delete iContainer; //just in case
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

	    _LIT(KBTServiceName, "Ookjor");
	    _LIT(KBTServiceDesc, "Ookjor");

		#define KBT_serviceID 0x2BC2B92E,0x399211DC, 0x83140800, 0x200C9A38
	    TUUID serviceUUID(KBT_serviceID);


	    iBtServer = new CBtServer(*this,KBTServiceName,KBTServiceDesc,serviceUUID);

	    iBtServer->StartServerL();

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
	CAknInformationNote* informationNote = new (ELeave) CAknInformationNote(ETrue);
		        	informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
		        	informationNote->ExecuteLD(aDesc);

}

