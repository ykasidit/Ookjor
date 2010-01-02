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
#include <eiklabel.h>

class COokjorAppUi;

class COokjorContainer : public CSkinnedContainer
    {
public:

	COokjorContainer();
    static COokjorContainer* NewL(const TRect& aRect);
    static COokjorContainer* NewLC(const TRect& aRect);
    ~COokjorContainer();

    virtual void ConstructL(const TRect& aRect);

	// from CoeControl, probably through
    virtual TInt CountComponentControls() const;
    virtual CCoeControl* ComponentControl(TInt aIndex) const;
	virtual void Draw(const TRect& aRect) const;
	virtual void SizeChanged();
	virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	/////////

    COokjorAppUi *appui;

	void SetStateL(const TDesC& aState);
	void SetStatusL(const TDesC& aState);
	void SetHintL(const TDesC& aState);

	CEikLabel* iStateLabel;//ready, connected, disconnected
    CEikLabel* iStatusLabel;//press connect on computer, sending frame, etc
    CEikLabel* iHintLabel;//Install on computer from www.ClearEvo.com

    CFbsBitmap* iBitmap;
    void LoadBitMap();
    };

#endif //
