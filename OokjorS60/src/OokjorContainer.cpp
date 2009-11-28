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
#include "OokjorContainer.h"
#include <iclmain.mbg>
#include <uikon.hrh>
#include <avkon.hrh>
#include <aknnotewrappers.h>



COokjorContainer* COokjorContainer::NewL(const TRect& aRect)
    {
		COokjorContainer* self = COokjorContainer::NewLC(aRect);
	    CleanupStack::Pop(self);
	    return self;
    }

COokjorContainer* COokjorContainer::NewLC(const TRect& aRect)
    {
	    COokjorContainer* self = new (ELeave) COokjorContainer();
	    CleanupStack::PushL(self);
	    self->ConstructL(aRect);
	    return self;
    }

void COokjorContainer::ConstructL(const TRect& aRect)
    {
	  LoadBitMap();
	  CSkinnedContainer::ConstructL();
   // CreateWindowL();
	//SetRect(aRect);
	//ActivateL();
    }

TInt COokjorContainer::CountComponentControls() const
    {
    return 0;
    }

COokjorContainer::~COokjorContainer()
{
	delete iBitmap;
	iBitmap = NULL;
}


void COokjorContainer::Draw(const TRect& arect) const
    {

	CSkinnedContainer::Draw(arect);

    // Get the standard graphics context
 	CWindowGc& gc = SystemGc();
    // Gets the control's extent
    TRect aRect = Rect();
//    gc.Clear(aRect);

	aRect.Width();
    TPoint p;
    if(iBitmap)
    {
    TSize picsz = iBitmap->SizeInPixels();

    p.iX = aRect.Width() - picsz.iWidth;
    p.iX/=2;

    p.iY = aRect.Height() - picsz.iHeight;
    p.iY/=2;

    if(iBitmap)
    {
		gc.BitBlt(p,iBitmap);
    }
    }

    }

CCoeControl* COokjorContainer::ComponentControl(TInt) const
    {
   		return NULL;
    }

COokjorContainer::COokjorContainer()
{
}

void COokjorContainer::AddLog(const TDesC& aLog)
{
}

TKeyResponse COokjorContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
	return EKeyWasNotConsumed;
}

void COokjorContainer::LoadBitMap()
	{
/*
		if(iBitmap==NULL)
		{
		TFileName filePath(_L("iclmain.mbm"));
	    #ifndef __WINS__
		COokjorAppUi::CompleteWithPrivatePathL(filePath);
	    #endif

		 iBitmap= new( ELeave )CFbsBitmap;
	  	 iBitmap->Load( filePath, EMbmIclmainMain, EMbmIclmainMain_mask);
		}*/


	}

