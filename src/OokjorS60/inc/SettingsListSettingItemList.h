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

#ifndef SETTINGSLISTSETTINGITEMLIST_H
#define SETTINGSLISTSETTINGITEMLIST_H

// INCLUDES

// System includes
#include <aknsettingitemlist.h> // CAknSettingItemList

// User includes
#include "Ookjor.hrh"
#include "SettingsListSettings.h"

// CLASS DECLARATION


class CAknView;
class CSettingsListSettingItemList : public CAknSettingItemList
	{
public: // Constructors and destructor
	CSettingsListSettingItemList(TSettingsListSettings& aSettings);

	~CSettingsListSettingItemList();

public: // from CAknSettingItemList
	CAknSettingItem* CreateSettingItemL(TInt identifier);
	void EditItemL (TInt aIndex, TBool aCalledFromMenu);
TKeyResponse	OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

public:// members
	void ChangeSelectedItemL ();

		TSettingsListSettings &iSettings;
private: // from CAknSettingItemList
	void SizeChanged();
	TInt autoLoad;





	};

#endif	// #ifndef SETTINGSLISTSETTINGITEMLIST_H

// End of File
