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
#include <uikon.hrh>
#include <avkon.hrh>
#include <aknnotewrappers.h>
#include <eikenv.h>
#include <ookjormain.mbg>

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
    CreateWindowL();
	CSkinnedContainer::ConstructL();

	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	TRgb NormalTextColor = KRgbGray;

	#ifdef EKA2
	AknsUtils::GetCachedColor(skin, NormalTextColor, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG6);
	#else
	TAknsItemID data;
	data.Set(EAknsMajorSkin, EAknsMinorQsnComponentColorBmpCG5);
	AknsUtils::GetCachedColor(skin, NormalTextColor, data, EAknsMinorQsnComponentColorBmpCG5);
	#endif



	 _LIT(KTextLoading, "Loading...");

	 LoadBitMap();

	 iStateLabel = new (ELeave) CEikLabel;
	 iStateLabel->SetContainerWindowL( *this );
	 iStateLabel->SetTextL(KTextLoading);
	 //////font
	 {
	 CFont* font = NULL;
	 font =  (CFont*) CEikonEnv::Static()->TitleFont();
	 if(font)
	 {
	 iStateLabel->SetFont(font);
	 }
	 }
	 //////
	 iStateLabel->OverrideColorL( EColorLabelText, NormalTextColor );

	 iStatusLabel = new (ELeave) CEikLabel;
	 iStatusLabel->SetContainerWindowL( *this );
	 iStatusLabel->SetTextL(KTextLoading);
	 {
	 //////font - http://wiki.forum.nokia.com/index.php/CS000833_-_Setting_font_for_CEikLabel
	 _LIT(KFontName, "LatinPlain12");
	 const TInt KFontSize = 140;  // Height of the typeface in twips
	 TFontSpec fontSpec(KFontName, KFontSize);
	 // Find the nearest available font to the TFontSpec and assign it to CFont
	 CGraphicsDevice* screenDevice = iEikonEnv->ScreenDevice();
	 CFont* font;
	 screenDevice->GetNearestFontInTwips(font, fontSpec);
	 // Set the font for the label
	 iStatusLabel->SetFont(font);
	 // Destroy the font
	 screenDevice->ReleaseFont(font);
	 //////
	 }
	 iStatusLabel->OverrideColorL( EColorLabelText, NormalTextColor );


	 iHintLabel = new (ELeave) CEikLabel;
	 iHintLabel->SetContainerWindowL( *this );
	 iHintLabel->SetTextL(KTextLoading);
	 {
	 //////font - http://wiki.forum.nokia.com/index.php/CS000833_-_Setting_font_for_CEikLabel
	 _LIT(KFontName, "LatinPlain12");
	 const TInt KFontSize = 140;  // Height of the typeface in twips
	 TFontSpec fontSpec(KFontName, KFontSize);
	 // Find the nearest available font to the TFontSpec and assign it to CFont
	 CGraphicsDevice* screenDevice = iEikonEnv->ScreenDevice();
	 CFont* font;
	 screenDevice->GetNearestFontInTwips(font, fontSpec);
	 // Set the font for the label
	 iHintLabel->SetFont(font);
	 // Destroy the font
	 screenDevice->ReleaseFont(font);
	 //////
	 }
	 iHintLabel->OverrideColorL( EColorLabelText,NormalTextColor);


	SetRect(aRect);
	ActivateL();
    }

void COokjorContainer::SizeChanged()
{

	CSkinnedContainer::SizeChanged();

	TInt w,h;
	w = Rect().Width();
	h = Rect().Height();

	if(iStateLabel)
	{
		TSize sz;
		sz = iStateLabel->MinimumSize();
		sz.iHeight += 5;
	iStateLabel->SetExtent(TPoint((w/2)-(sz.iWidth/2),h/7), sz);
	sz = iStatusLabel->MinimumSize();
			sz.iHeight += 5;
	iStatusLabel->SetExtent(TPoint((w/2)-(sz.iWidth/2),5*(h/7)), sz);
	sz = iHintLabel->MinimumSize();
			sz.iHeight += 5;
	iHintLabel->SetExtent(TPoint((w/2)-(sz.iWidth/2),6*(h/7)), sz);
	}
}

TInt COokjorContainer::CountComponentControls() const
    {
    return 3; //3 labels
    }

COokjorContainer::~COokjorContainer()
{
	delete iBitmap;
}

void COokjorContainer::LoadBitMap()
	{

		if(iBitmap==NULL)
		{
		TFileName filePath(_L("C:\\private\\A000EC71\\ookjormain.mbm"));
		 iBitmap= new( ELeave )CFbsBitmap;
	  	 User::LeaveIfError(iBitmap->Load( filePath, EMbmOokjormainOkjr_combtn));
		}


	}

void COokjorContainer::Draw(const TRect& arect) const
    {
		CSkinnedContainer::Draw(arect);

		if(iBitmap)
		 {
			TRect aRect = Rect();
			CWindowGc& gc = SystemGc();
				aRect.Width();
				TPoint p;
				TSize picsz = iBitmap->SizeInPixels();

				p.iX = aRect.Width() - picsz.iWidth;
				p.iX/=2;

				p.iY = aRect.Height() - picsz.iHeight;
				p.iY/=2;

		 	gc.BitBlt(p,iBitmap);
		 }
    }

CCoeControl* COokjorContainer::ComponentControl(TInt index) const
    {
		switch(index)
		{
		case 0:
			return iStateLabel;
		case 1:
			return iStatusLabel;
		case 2:
			return iHintLabel;
		}
   		return NULL;
    }

COokjorContainer::COokjorContainer()
{
}



TKeyResponse COokjorContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
	return EKeyWasNotConsumed;
}






void COokjorContainer::SetStateL(const TDesC& aState)
{
	if(iStateLabel)
	{
		iStateLabel->SetTextL(aState);
	}
}

void COokjorContainer::SetStatusL(const TDesC& aState)
{
	if(iStatusLabel)
		iStatusLabel->SetTextL(aState);
}

void COokjorContainer::SetHintL(const TDesC& aState)
{
	if(iHintLabel)
		iHintLabel->SetTextL(aState);
}

