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

#ifndef MYRICHTEXTEDITOR_H
#define MYRICHTEXTEDITOR_H




#include <eikrted.h> // CCEikRichTextEditor
#include <txtfrmat.h> // TCharFormatMask

enum TExtrafonts
{
	ETitleFont=100,
	EAnnotationFont,
	ELegendFont,
	ESymbolFont,
	EDenseFont
};

class CMyRichTextEditor : public CEikRichTextEditor
	{
public:

	static CMyRichTextEditor* NewL();
	static CMyRichTextEditor* NewLC();
	~CMyRichTextEditor();

private: // Constructor

	void ConstructL();

public: // from CoeControl

	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

public: // members

	void SetUnderlineOn (TBool aOn);
	void SetBoldOn (TBool aOn);
	void AddEndOfParagraphL ();
	void AddTextL (const TDesC& aText);
	void DisplayStartOfHelpTextL();
	void SetSystemFont(TInt fontID);
	void SetColor(TRgb color);
	void SetAlign(CParaFormat::TAlignment alignID);
	void AddLineBreakL();

	void AddURLHighlightedText(const TDesC& aText);

	//data

	TCharFormatMask iCharFormatMask;  // current formatting, e.g bold etc
	TCharFormat iCharFormat; // current formatting, e.g bold etc
	CParaFormat* iParaFormat;
	TParaFormatMask iParaFormatMask;





	};

#endif


