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


#include "AboutAppView.h"
#include "ScrollRichTextContainer.h"
#include <aknviewappui.h>
#include <Ookjor.rsg>
#include "Ookjor.hrh"
#include <avkon.rsg>
#include <eikbtgpc.h>
#include <aknnotewrappers.h>
#include "AboutTextContainer.h"

TUid CAboutAppView::Id() const
	{
	   return iId;
	}

CAknView* CAboutAppView::NewL(TInt viewID)
	{
	    CAknView* self = CAboutAppView::NewLC(viewID);
	    CleanupStack::Pop(self);
	    return self;
    }

CAknView* CAboutAppView::NewLC(TInt viewID)
    {
	    CAknView* self = new (ELeave) CAboutAppView(viewID);
	    CleanupStack::PushL(self);
	    ((CAboutAppView*) self)->ConstructL();
	    return self;
    }


CAboutAppView::CAboutAppView(TInt viewID)
    {
		iId = TUid::Uid(viewID);
    }

CAboutAppView::~CAboutAppView()
    {

    }

void CAboutAppView::ConstructL()
    {
	    BaseConstructL(R_ABOUT_VIEW);
    }


void CAboutAppView::HandleCommandL(TInt aCommand)
    {
				AppUi()->HandleCommandL(aCommand);
    }

void CAboutAppView::DoActivateL(const TVwsViewId& /*aPrevViewId*/,
                                    TUid /*aCustomMessageId*/,
                                    const TDesC8& /*aCustomMessage*/)
    {

		ASSERT(iContainer == NULL);

		iContainer = CAboutTextContainer::NewL(ClientRect(),this);
		AppUi()->AddToStackL(*this,iContainer);



    }

void CAboutAppView::DoDeactivate()
    {
       if (iContainer)
        {
        AppUi()->RemoveFromStack(iContainer);
        delete iContainer;
        iContainer = NULL;
        }
    }




