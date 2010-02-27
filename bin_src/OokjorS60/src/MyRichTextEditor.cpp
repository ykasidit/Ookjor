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


#include "MyRichTextEditor.h"
#include <eikedwin.h>
#include <eikgted.h>

#include <barsread.h> // TResourceReader
#include <Ookjor.rsg> // resources
#include <eikrted.h> // CEikRichTextEditor
#include <gdi.h> // TFontStyle
#include <txtrich.h> // CRichText
#include <avkon.hrh>
#include <aknutils.h>  // for Fonts.

CMyRichTextEditor::~CMyRichTextEditor()
{
	delete iParaFormat;
}

void CMyRichTextEditor::ConstructL()
	{

	TResourceReader reader;
	iCoeEnv->CreateResourceReaderLC(reader, R_MYRICHTEXTEDITOR_EDITOR);
	ConstructFromResourceL(reader);
	CleanupStack::PopAndDestroy(); // reader

	iParaFormat = CParaFormat::NewL();



	SetAknEditorFlags
		(
		EAknEditorFlagEnableScrollBars 	// Set up the scroller
		);


	SetFocus(ETrue);



	}




void CMyRichTextEditor::DisplayStartOfHelpTextL()
	{
	SetCursorPosL(0, EFalse);
	}
void CMyRichTextEditor::SetColor(TRgb color)
{
	//3rd themes are stong colored

/*	#ifdef EKA2
		color.SetRed(255-color.Red());
		color.SetGreen(255-color.Green());
		color.SetBlue(255-color.Blue());
	#endif*/

	iCharFormat.iFontPresentation.iTextColor = color;
	iCharFormatMask.SetAll();

}


void CMyRichTextEditor::SetSystemFont(TInt aFontEnum)
{
	 const CFont* editorFont ;


    switch (aFontEnum)
        {
        case ELatinPlain12:
            editorFont = LatinPlain12();
            break;
        case ELatinBold12:
            editorFont = LatinBold12();
            break;
        case ELatinBold13:
            editorFont = LatinBold13();
            break;
        case ELatinBold17:
            editorFont = LatinBold17();
            break;
        case ELatinBold19:
            editorFont = LatinBold19();
            break;
        case ENumberPlain5:
            editorFont = NumberPlain5();
            break;
        case EClockBold30:
            editorFont = ClockBold30();
            break;
        case ELatinClock14:
            editorFont = LatinClock14();
            break;

       case ETitleFont:
       {


	#ifdef EKA2
//	        editorFont = CEikonEnv::Static()->TitleFont();
//	       editorFont = CEikonEnv::Static()->TitleFont();
			TBuf<128> font;
			TTypefaceSupport myTypefaceSupport;
			iCoeEnv->ScreenDevice()->TypefaceSupport(myTypefaceSupport,0);
			font = myTypefaceSupport.iTypeface.iName.Des();
			TFontSpec fontSpec(font,195);
			fontSpec.iFontStyle.SetStrokeWeight(EStrokeWeightBold);
			fontSpec.iFontStyle.SetPosture( EPostureUpright );
			CGraphicsDevice* screenDevice=iCoeEnv->ScreenDevice();
			CFont* myFont;
			screenDevice->GetNearestFontInTwips(myFont,fontSpec);
			editorFont = myFont;
			screenDevice->ReleaseFont(myFont);

	#else
			editorFont = LatinBold13();
	#endif
        }
        break;

		case EAnnotationFont:
		{



	#ifdef EKA2

			TBuf<128> font;
			TTypefaceSupport myTypefaceSupport;
			iCoeEnv->ScreenDevice()->TypefaceSupport(myTypefaceSupport,0);
			font = myTypefaceSupport.iTypeface.iName.Des();
			TFontSpec fontSpec(font,170);
			fontSpec.iFontStyle.SetStrokeWeight(EStrokeWeightBold);
			fontSpec.iFontStyle.SetPosture( EPostureUpright );
			CGraphicsDevice* screenDevice=iCoeEnv->ScreenDevice();
			CFont* myFont;
			screenDevice->GetNearestFontInTwips(myFont,fontSpec);
			editorFont = myFont;
			screenDevice->ReleaseFont(myFont);




//editorFont = (CFont *)AknLayoutUtils::FontFromId(EAknLogicalFontPrimaryFont);
//		editorFont = (CFont *)AknLayoutUtils::FontFromId(EAknLogicalFontPrimaryFont);

	#else
       editorFont = CEikonEnv::Static()->TitleFont();
	#endif

		}
    	break;

#ifdef EKA2
		case ESymbolFont:
		#endif
		case ELegendFont:
		{


	#ifdef EKA2

//	        editorFont = (CFont *)AknLayoutUtils::FontFromId(EAknLogicalFontPrimaryFont);

TBuf<128> font;
			TTypefaceSupport myTypefaceSupport;
			iCoeEnv->ScreenDevice()->TypefaceSupport(myTypefaceSupport,0);
			font = myTypefaceSupport.iTypeface.iName.Des();
			TFontSpec fontSpec(font,150);
			fontSpec.iFontStyle.SetStrokeWeight(EStrokeWeightNormal);
			fontSpec.iFontStyle.SetPosture( EPostureUpright );
			CGraphicsDevice* screenDevice=iCoeEnv->ScreenDevice();
			CFont* myFont;
			screenDevice->GetNearestFontInTwips(myFont,fontSpec);
			editorFont = myFont;
			screenDevice->ReleaseFont(myFont);

	#else
			editorFont = LatinBold12();
	#endif
		}
         break;

#ifndef EKA2
		case ESymbolFont:
			editorFont = LatinPlain12();
	#endif
         break;

		case EDenseFont:
       default:
	         editorFont = CEikonEnv::Static()->DenseFont();

	    break;


            return;
        }


    iCharFormat.iFontSpec = editorFont->FontSpecInTwips();

//	iCharFormatMask.SetAll();

	iParaFormat->iLineSpacingInTwips = 1;
//	iParaFormatMask.SetAll();




}


void CMyRichTextEditor::AddURLHighlightedText(const TDesC& aText)
{

	CRichText* richText = RichText();
	TInt documentLength = richText->DocumentLength();
	richText->InsertL(documentLength, aText);

//	richText->ApplyCharFormatL(iCharFormat, iCharFormatMask,documentLength,aText.Length());
//	richText->ApplyParaFormatL(iParaFormat,iParaFormatMask,documentLength,aText.Length());

	SetSelectionL(documentLength,aText.Length());



	iCharFormat.iFontPresentation.iHighlightColor=TRgb(153, 221, 255);
	iCharFormat.iFontPresentation.iHighlightStyle=TFontPresentation::EFontHighlightNormal;
	iCharFormatMask.SetAttrib(EAttFontHighlightColor );
	iCharFormatMask.SetAttrib(EAttFontHighlightStyle );
	richText->ApplyCharFormatL(iCharFormat, iCharFormatMask,documentLength,aText.Length());
	ClearSelectionL();

	iCharFormatMask.ClearAll();

		iCharFormat.iFontPresentation.iHighlightStyle=TFontPresentation::EFontHighlightNone;

}

void CMyRichTextEditor::SetUnderlineOn(TBool aOn)
	{

	iCharFormatMask.SetAttrib(EAttFontUnderline);
	if (aOn)
		{
		iCharFormat.iFontPresentation.iUnderline = EUnderlineOn;
		}
	else
		{
		iCharFormat.iFontPresentation.iUnderline = EUnderlineOff;
		}
	}
void CMyRichTextEditor::SetAlign(CParaFormat::TAlignment aid)
{
		iParaFormatMask.SetAttrib(EAttAlignment); // interested in alignment


		iParaFormat->iHorizontalAlignment= aid;
}
void CMyRichTextEditor::SetBoldOn(TBool aOn)
	{
	iCharFormatMask.SetAttrib(EAttFontStrokeWeight);

	if (aOn)
		{
		iCharFormat.iFontSpec.iFontStyle.SetStrokeWeight(EStrokeWeightBold);
		}
	else
		{
		iCharFormat.iFontSpec.iFontStyle.SetStrokeWeight(EStrokeWeightNormal);
		}
	}

void CMyRichTextEditor::AddEndOfParagraphL()
	{
	CRichText* richText = RichText();
	richText->InsertL(richText->DocumentLength(), CEditableText::EParagraphDelimiter);

	}

void CMyRichTextEditor::AddLineBreakL()
	{
	CRichText* richText = RichText();
	richText->InsertL(richText->DocumentLength(), CEditableText::ELineBreak);

	}


void CMyRichTextEditor::AddTextL(const TDesC& aText)
	{
	CRichText* richText = RichText();
	TInt documentLength = richText->DocumentLength();
	richText->InsertL (documentLength, aText);

	richText->ApplyCharFormatL(iCharFormat, iCharFormatMask,documentLength,aText.Length());
	richText->ApplyParaFormatL(iParaFormat,iParaFormatMask,documentLength,aText.Length());



	}


CMyRichTextEditor* CMyRichTextEditor::NewL()
	{
	CMyRichTextEditor* self = CMyRichTextEditor::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

CMyRichTextEditor* CMyRichTextEditor::NewLC()
	{
	CMyRichTextEditor* self = new (ELeave) CMyRichTextEditor;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

TKeyResponse CMyRichTextEditor::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
	{



    if (aType == EEventKey)
		{
        if (aKeyEvent.iCode == EKeyDownArrow )
            {

MoveCursorL(TCursorPosition::EFPageDown, EFalse);
MoveCursorL(TCursorPosition::EFLineBeg, EFalse);
UpdateScrollBarsL();


            return EKeyWasConsumed;
            }
        else if (aKeyEvent.iCode == EKeyUpArrow )
            {

MoveCursorL(TCursorPosition::EFPageUp, EFalse);
MoveCursorL(TCursorPosition::EFLineBeg, EFalse);
UpdateScrollBarsL();

            return EKeyWasConsumed;

            }
		/*else
			{

            return CEikRichTextEditor::OfferKeyEventL(aKeyEvent, aType);;

			}*/
        }

    return EKeyWasNotConsumed;


	}

