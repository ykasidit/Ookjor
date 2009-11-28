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
#include "HelpTextContainer.h"
#include <avkon.hrh>
// System includes
#include <barsread.h> // TResourceReader
#include <Ookjor.rsg>
#include <stringloader.h>
#include <eikenv.h>
#include "OokjorAppui.h"


// User includes
#include "MyRichTextEditor.h"

#define KEditorPosition TPoint(0,0)


void CHelpTextContainer::AddTextToEditorL()
	{
		MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	TRgb NormalTextColor = KRgbBlack;


	#ifdef EKA2
	AknsUtils::GetCachedColor(skin, NormalTextColor, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG6);
	#else
	TAknsItemID data;
	data.Set(EAknsMajorSkin, EAknsMinorQsnComponentColorBmpCG5);
	AknsUtils::GetCachedColor(skin, NormalTextColor, data, EAknsMinorQsnComponentColorBmpCG5);
	#endif

	if (iEditor)
		{

		iEditor->AddLineBreakL();
		iEditor->SetAlign(CParaFormat::ELeftAlign);
		HBufC* header1 = StringLoader::LoadLC (R_HELPINTRO_1STR);
		iEditor->SetUnderlineOn(EFalse);
		iEditor->SetBoldOn(ETrue);
		iEditor->SetSystemFont(EAnnotationFont);
		iEditor->SetColor(NormalTextColor);
//		iEditor->SetBackgroundColorL(KRgbBlack);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();
		iEditor->AddEndOfParagraphL();

		iEditor->SetBoldOn(ETrue);
		header1 = StringLoader::LoadLC (R_OOKJORHELPEND_STR);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();
		iEditor->AddEndOfParagraphL();

		iEditor->SetBoldOn(EFalse);


		header1 = StringLoader::LoadLC (R_HELPINTRO_2STR);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();
		iEditor->AddEndOfParagraphL();


		header1 = StringLoader::LoadLC (R_HELPDETAILS0_STR);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();
		iEditor->AddEndOfParagraphL();

		header1 = StringLoader::LoadLC (R_HELPDETAILS1_STR);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();
		iEditor->AddEndOfParagraphL();

		header1 = StringLoader::LoadLC (R_HELPDETAILS2_STR);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();
		iEditor->AddEndOfParagraphL();

		iEditor->SetBoldOn(ETrue);

		header1 = StringLoader::LoadLC (R_SETTINGS_STR);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();

		iEditor->SetBoldOn(EFalse);

		header1 = StringLoader::LoadLC (R_HELPDETAILS3_0_STR);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();
		iEditor->AddEndOfParagraphL();


		header1 = StringLoader::LoadLC (R_HELPDETAILS3_STR);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();
		iEditor->AddEndOfParagraphL();

		header1 = StringLoader::LoadLC (R_HELPDETAILS4_STR);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();
		iEditor->AddEndOfParagraphL();

		header1 = StringLoader::LoadLC (R_HELPDETAILS5_STR);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();
		iEditor->AddEndOfParagraphL();

		iEditor->SetBoldOn(EFalse);
		header1 = StringLoader::LoadLC (R_NOTEONALERTS_STR);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();
		iEditor->AddEndOfParagraphL();


		}
	}

CHelpTextContainer* CHelpTextContainer::NewL(const TRect& aRect,MObjectProvider *provider)
	{
	CHelpTextContainer* self = CHelpTextContainer::NewLC(aRect,provider);
	CleanupStack::Pop(self);
	return self;
	}

CHelpTextContainer* CHelpTextContainer::NewLC(const TRect& aRect,MObjectProvider *provider)
	{
	CHelpTextContainer* self = new (ELeave) CHelpTextContainer(provider);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self;
	}

CHelpTextContainer::CHelpTextContainer(MObjectProvider *provider):CScrollRichTextContainer(provider)
{

}
