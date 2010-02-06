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


#ifndef __Ookjor_SETTINGSVIEW_H__
#define __Ookjor_SETTINGSVIEW_H__

#include <aknview.h>
#include "SettingsListSettings.h"

class COokjorAppUi;
class COokjorSettingsContainer;



class COokjorSettingsView :  public CAknView
    {
public:

	COokjorAppUi *appui;
	TUid Id() const;
	void HandleCommandL(TInt aCommand);
  	static COokjorSettingsView* NewL(COokjorAppUi *ui);
  	static COokjorSettingsView* NewLC(COokjorAppUi *ui);
    ~COokjorSettingsView();
    //void HandleForegroundEventL(TBool fg);
	//void HandleStatusPaneSizeChange();
	void DoActivateL(const TVwsViewId& aPrevViewId,
                   TUid aCustomMessageId,
                   const TDesC8& aCustomMessage);
	void DoDeactivate();
	void SaveSettingsL();
	TSettingsListSettings iSettings;

private:
    COokjorSettingsView(COokjorAppUi *ui);
    void ConstructL();
    CCoeControl *iContainer;
    TUid iId;

    };


#endif // __Ookjor_SETTINGSVIEW_H__
