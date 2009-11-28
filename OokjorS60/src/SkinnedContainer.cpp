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

#include "SkinnedContainer.h"
#include <eikenv.h>
#include <eikappui.h>
#include <akndef.h>
#include <aknutils.h>

CSkinnedContainer::~CSkinnedContainer()
{
	delete iBgContext;
	iBgContext = NULL;
}



void CSkinnedContainer::ConstructL(void)
   {
    CreateWindowL();

	// make first with no size at all
	iBgContext = CAknsBasicBackgroundControlContext::NewL(
		KAknsIIDQsnBgScreen,TRect(0,0,1,1), ETrue);
	// Setting rect will cause SizeChanged to be called
	// and iBgContext size & position is updated accordingly.
	SetRect(CEikonEnv::Static()->EikAppUi()->ClientRect());

	ActivateL();
	DrawNow();
}

void CSkinnedContainer::SizeChanged()
{
	if ( iBgContext )
	{
		iBgContext->SetRect(Rect());

		if ( &Window() )
		{
			iBgContext->SetParentPos( PositionRelativeToScreen() );
		}
	}
}


void CSkinnedContainer::HandleResourceChange(TInt aType)
{
	TRect rect;

    if ( aType==KEikDynamicLayoutVariantSwitch )
    {
        AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, rect);
        SetRect(rect);
    }

	CCoeControl::HandleResourceChange(aType);
}


TTypeUid::Ptr CSkinnedContainer::MopSupplyObject(TTypeUid aId)
{
	if (iBgContext)
	{
		return MAknsControlContext::SupplyMopObject(aId, iBgContext );
	}

	return CCoeControl::MopSupplyObject(aId);
}


void CSkinnedContainer::Draw(const TRect& aRect) const
{
	CWindowGc& gc = SystemGc();

	// draw background skin first.
  	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	AknsDrawUtils::Background( skin, iBgContext, this, gc, aRect );
}
