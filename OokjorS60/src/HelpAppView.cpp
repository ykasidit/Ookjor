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


#include "HelpAppView.h"
#include "HelpTextContainer.h"
#include "ScrollRichTextContainer.h"
#include <aknviewappui.h>
#include <Ookjor.rsg>
#include "Ookjor.hrh"
#include <avkon.rsg>
#include <eikbtgpc.h>


#include "HelpTextContainer.h"

TUid CHelpAppView::Id() const
	{
	   return iId;
	}

CAknView* CHelpAppView::NewL(TInt viewID)
	{
	    CAknView* self = CHelpAppView::NewLC(viewID);
	    CleanupStack::Pop(self);
	    return self;
    }

CAknView* CHelpAppView::NewLC(TInt viewID)
    {
	    CAknView* self = new (ELeave) CHelpAppView(viewID);
	    CleanupStack::PushL(self);
	    ((CHelpAppView*) self)->ConstructL();
	    return self;
    }


CHelpAppView::CHelpAppView(TInt viewID)
    {
		iId = TUid::Uid(viewID);
    }

CHelpAppView::~CHelpAppView()
    {

    }

void CHelpAppView::ConstructL()
    {
	    BaseConstructL(R_HELP_VIEW);
    }


void CHelpAppView::HandleCommandL(TInt aCommand)
    {
				AppUi()->HandleCommandL(aCommand);
    }

void CHelpAppView::DoActivateL(const TVwsViewId& /*aPrevViewId*/,
                                    TUid /*aCustomMessageId*/,
                                    const TDesC8& /*aCustomMessage*/)
    {

		ASSERT(iContainer == NULL);


		iContainer = CHelpTextContainer::NewL(ClientRect(),this);

		AppUi()->AddToStackL(*this,iContainer);



    }

void CHelpAppView::DoDeactivate()
    {
       if (iContainer)
        {
        AppUi()->RemoveFromStack(iContainer);
        delete iContainer;
        iContainer = NULL;
        }
    }




