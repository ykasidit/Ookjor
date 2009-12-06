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

#ifndef __Ookjor_APPUI_H__
#define __Ookjor_APPUI_H__

#include <aknviewappui.h>
#include <akntabgrp.h>
#include <aknnavide.h>


// Forward reference
class COokjorAppView;
class COokjorSettingsView;


/*!
  @class COokjorAppUi

  @discussion An instance of class COokjorAppUi is the UserInterface part of the AVKON
  application framework for the Ookjor example application
  */
class COokjorAppUi : public CAknViewAppUi
    {
public:

/*!
  @function ConstructL

  @discussion Perform the second phase construction of a COokjorAppUi object
  this needs to be public due to the way the framework constructs the AppUi
  */
    void ConstructL();


/*!
  @function COokjorAppUi

  @discussion Perform the first phase of two phase construction.
  This needs to be public due to the way the framework constructs the AppUi
  */
    COokjorAppUi();


/*!
  @function ~COokjorAppUi

  @discussion Destroy the object and release all memory objects
  */
    ~COokjorAppUi();

    static void CompleteWithPrivatePathL(TDes& des)
    {
    #ifdef EKA2//3rd edition
    	RFs rfs;
    	TFileName fn;

    	User::LeaveIfError(rfs.Connect());
    	CleanupClosePushL(rfs);
    	User::LeaveIfError(rfs.PrivatePath(fn));
    	CleanupStack::PopAndDestroy();
    	fn += des;
    	des = fn;
    #else
    	CompleteWithAppPath(des);
    #endif
    }

void HandleWsEventL(const TWsEvent &aEvent, CCoeControl *aDestination);//handle red-key press


public: // from CAknAppUi
/*!
  @function HandleCommandL

  @discussion Handle user menu selections
  @param aCommand the enumerated code for the option selected
  */
    void HandleCommandL(TInt aCommand);

//private:
/*! @var iAppView The application view */
    COokjorAppView* iAppView;
    CAknView *iAboutAppView;

    TBool iLayout;
    TBool iVisible;

    //CAknNavigationControlContainer* iNaviPane;
    //CAknTabGroup*                   iTabGroup;
    //CAknNavigationDecorator*        iDecoratedTabGroup;


    void DoConstuctL();

    };


#endif // __Ookjor_APPUI_H__

