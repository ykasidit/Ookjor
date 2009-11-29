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


#ifndef OokjorCONTAINER_H
#define OokjorCONTAINER_H

#include "SkinnedContainer.h"
#include <aknview.h>

class COokjorAppUi;

class COokjorContainer : public CSkinnedContainer
    {
public:

	COokjorContainer();
    static COokjorContainer* NewL(const TRect& aRect);
    static COokjorContainer* NewLC(const TRect& aRect);
    ~COokjorContainer();

    virtual void ConstructL(const TRect& aRect);

	// from CoeControl
     TInt CountComponentControls() const;
     CCoeControl* ComponentControl(TInt aIndex) const;
	virtual void Draw(const TRect& aRect) const;

	TUint8 GetOokjorCommandCode(TUint keyCode);
	virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	void AddLog(const TDesC& aLog);

    COokjorAppUi *appui;
	CFbsBitmap* iBitmap;
	void LoadBitMap();

    TBuf8<6> iSendBuf;
    };

#endif //
