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


#ifndef OokjorSETTINGSCONTAINER_H
#define OokjorSETTINGSCONTAINER_H

#include <aknview.h>
#include "SettingsListSettings.h"

class COokjorAppUi;
class CSettingsListSettingItemList;


class COokjorSettingsContainer : public CCoeControl
    {
public:

    static COokjorSettingsContainer* NewL(const TRect& aRect,COokjorAppUi *ui);
    static COokjorSettingsContainer* NewLC(const TRect& aRect,COokjorAppUi *ui);
    void ConstructL(const TRect& aRect);

	// from CoeControl
    TInt CountComponentControls() const;
	CCoeControl* ComponentControl(TInt aIndex) const;
	void Draw(const TRect& aRect) const;

   ~COokjorSettingsContainer();
    COokjorSettingsContainer(COokjorAppUi *ui);
    CSettingsListSettingItemList *iSettingItemList;
    TSettingsListSettings iSettings;
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
    void ChangeSelectedItemL();

	static void GetIniPath(TFileName & aIniFileName);
	void SaveSettingsL();
    };

#endif //
