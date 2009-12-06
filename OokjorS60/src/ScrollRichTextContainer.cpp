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
#include "ScrollRichTextContainer.h"
#include <avkon.hrh>
// System includes
#include <barsread.h> // TResourceReader
#include <Ookjor.rsg> // resources
#include <stringloader.h>
#include <eikenv.h>
#include <txtfrmat.h>
#include <aknutils.h>  // for Fonts.
#include <aknsutils.h>
#include <aknsdrawutils.h>

#include <aknsdrawutils.h>
#include <aknsbasicbackgroundcontrolcontext.h>


// User includes
#include "MyRichTextEditor.h"

#define KEditorPosition TPoint(0,0)

void CScrollRichTextContainer::ConstructL(const TRect& aRect)
	{
	CreateWindowL();

	iBackGround = CAknsBasicBackgroundControlContext::NewL( KAknsIIDQsnBgAreaMain,aRect,ETrue);

	iEditor = CMyRichTextEditor::NewL();
	iEditor->SetContainerWindowL(*this);
	iEditor->CreateScrollBarFrameL();
	iEditor->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
	iEditor->SetRect(aRect);
//	iEditor->ActivateL();


	iEditor->iParaFormat->iLeftMarginInTwips = 100;
	iEditor->iParaFormatMask.SetAttrib(EAttLeftMargin);
	AddTextToEditorL();

	SetRect(aRect);
	ActivateL();

	iEditor->DisplayStartOfHelpTextL();

	SizeChanged();

	}


void CScrollRichTextContainer::AddTextToEditorL()
	{

	}

CScrollRichTextContainer::CScrollRichTextContainer(MObjectProvider *provider)
{
	SetMopParent(provider);
}

TTypeUid::Ptr CScrollRichTextContainer::MopSupplyObject(TTypeUid aId)
{
 if (iBackGround )
 {
   return MAknsControlContext::SupplyMopObject( aId, iBackGround );
 }
 return CCoeControl::MopSupplyObject(aId);
}

void CScrollRichTextContainer::Draw(const TRect& aRect) const
{

CWindowGc& gc = SystemGc();
gc.SetBrushStyle( CGraphicsContext::ESolidBrush );

MAknsSkinInstance* skin = AknsUtils::SkinInstance();
MAknsControlContext* cc = AknsDrawUtils::ControlContext(this);

gc.Clear(aRect);

AknsDrawUtils::Background(skin, cc,this,gc,aRect);

//	TRgb iNormalTextColor;
//	AknsUtils::GetCachedColor(skin, iNormalTextColor, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG6);

//gc.DrawRect( aRect );

}

CScrollRichTextContainer::~CScrollRichTextContainer()
	{
		delete iEditor;
		delete iBackGround;
	}


/*void CScrollRichTextContainer::SizeChanged()
	{
	iEditor->SetExtent(KEditorPosition, iEditor->MinimumSize());
	}*/

TInt CScrollRichTextContainer::CountComponentControls() const
	{
	return 1; // return number of controls inside this container
	}

CCoeControl* CScrollRichTextContainer::ComponentControl(TInt aIndex) const
	{
		switch (aIndex)
		{
		case 0:
			return iEditor;
		default:
			return NULL;
		}
	}



TKeyResponse CScrollRichTextContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
	{
	if (iEditor)
		return iEditor->OfferKeyEventL(aKeyEvent, aType);
	else
		return CCoeControl::OfferKeyEventL(aKeyEvent, aType);
	}

void CScrollRichTextContainer::SizeChanged()
    {
	//In 3rd edition CEikRichTextEditor draw the view for the whole rect and
	//the scrollbar doesn't show. That is a reason why CEikRichTextEditor Width() is
	//rect.Width()-ScrollBarRect.Width()
	TRect rect = Rect();



	#ifdef EKA2
	TRect ScrollBarRect = iEditor->ScrollBarFrame()->VerticalScrollBar()->Rect();
	iEditor->SetExtent(TPoint(0,0),
		TSize(rect.Width()-ScrollBarRect.Width(), rect.Height()));
	#else
    	iEditor->SetExtent(TPoint(0,0),
    	TSize(rect.Width(), rect.Height()));
    #endif
    }

void CScrollRichTextContainer::HandleResourceChange( TInt aType )
    {

	//http://wiki.forum.nokia.com/index.php/How_to_support_layout_switching

    CCoeControl::HandleResourceChange( aType );

    // application layout change request notification
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        // reconstruct controls if needed

        // apply new appropriate rect
        TRect rect;
        AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, rect );
        SetRect( rect );
        }
    }
