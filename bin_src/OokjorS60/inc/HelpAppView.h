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

#ifndef __HelpAPPVIEW_H__
#define __HelpAPPVIEW_H__

#include <aknview.h>


class CScrollRichTextContainer;

class CHelpAppView :  public CAknView
    {
public:

	TUid Id() const;
	void HandleCommandL(TInt aCommand);
  	 static  CAknView* NewL(TInt viewID);
  	 static  CAknView* NewLC(TInt viewID);
    ~CHelpAppView();
	void DoActivateL(const TVwsViewId& aPrevViewId,
                   TUid aCustomMessageId,
                   const TDesC8& aCustomMessage);

	void DoDeactivate();
	CScrollRichTextContainer *iContainer;
	CHelpAppView(TInt viewID);

protected:

    virtual void ConstructL();
    TUid iId;

    };


#endif
