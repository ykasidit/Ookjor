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
#include "AboutTextContainer.h"
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


void CAboutTextContainer::AddTextToEditorL()
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
		HBufC* header1 = StringLoader::LoadLC (R_APPVER_STR);
		iEditor->SetUnderlineOn(EFalse);
		iEditor->SetBoldOn(EFalse);
		iEditor->SetSystemFont(EAnnotationFont);
		iEditor->SetColor(KRgbBlue);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();

		iEditor->SetColor(KRgbGray);
		iEditor->SetSystemFont(EAnnotationFont);
		header1 = StringLoader::LoadLC(R_FORS60_STR);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();

		iEditor->SetSystemFont(EAnnotationFont);
		iEditor->SetColor(NormalTextColor);
		header1 = StringLoader::LoadLC (R_EDITION_STR);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();
		iEditor->AddEndOfParagraphL();


		iEditor->SetSystemFont(EAnnotationFont);
		iEditor->SetAlign(CParaFormat::ELeftAlign);


		header1 = StringLoader::LoadLC(R_ABOUT_STR);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();
		iEditor->AddEndOfParagraphL();

		header1 = StringLoader::LoadLC(R_HOWTO_STR);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();
		iEditor->AddEndOfParagraphL();

		header1 = StringLoader::LoadLC(R_DISCLAIMER_STR);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();
		iEditor->AddEndOfParagraphL();

		header1 = StringLoader::LoadLC(R_INFO_STR);
		iEditor->AddTextL (*header1);
		CleanupStack::PopAndDestroy(header1);
		iEditor->AddLineBreakL();
		iEditor->AddEndOfParagraphL();

		}
	}

CAboutTextContainer* CAboutTextContainer::NewL(const TRect& aRect,MObjectProvider *provider)
	{
	CAboutTextContainer* self = CAboutTextContainer::NewLC(aRect,provider);
	CleanupStack::Pop(self);
	return self;
	}

CAboutTextContainer* CAboutTextContainer::NewLC(const TRect& aRect,MObjectProvider *provider)
	{
	CAboutTextContainer* self = new (ELeave) CAboutTextContainer(provider);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self;
	}

CAboutTextContainer::CAboutTextContainer(MObjectProvider *provider):CScrollRichTextContainer(provider)
{

}
