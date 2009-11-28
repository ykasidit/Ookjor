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


#ifndef ScrollRichTextContainer_H
#define ScrollRichTextContainer_H



// System includes
#include <coecntrl.h> // CCoeControl
#include <txtfrmat.h> // TCharFormatMask
#include <aknsutils.h>
#include <aknsdrawutils.h>

class CMyRichTextEditor;


class CScrollRichTextContainer : public CCoeControl
	{
public: // Constructors and destructor

	~CScrollRichTextContainer();
	CScrollRichTextContainer(MObjectProvider *provider);

	void SizeChanged();

protected:

	void ConstructL(const TRect& aRect);

	TInt CountComponentControls() const;
	CCoeControl* ComponentControl(TInt aIndex) const;

	void Draw(const TRect& aRect) const;

	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

	virtual void AddTextToEditorL(); //just implement this one again for each use

	MAknsControlContext* iBackGround; // for skins support
	TTypeUid::Ptr MopSupplyObject(TTypeUid aId);


	CMyRichTextEditor* iEditor;
	};

#endif
