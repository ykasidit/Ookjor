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

#ifndef INCALLERTCONTAINER_H
#define INCALLERTCONTAINER_H

#include <aknview.h>

class CIncallertAppUi;

class CIncallertContainer : public CCoeControl
    {
public:

	CIncallertContainer();
    static CIncallertContainer* NewL(const TRect& aRect);
    static CIncallertContainer* NewLC(const TRect& aRect);
    ~CIncallertContainer();

    void ConstructL(const TRect& aRect);


	// from CoeControl
    TInt CountComponentControls() const;
	CCoeControl* ComponentControl(TInt aIndex) const;
	void Draw(const TRect& aRect) const;

    TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

            CFbsBitmap* iBitmap;
            void LoadBitMap();

    };

#endif //
