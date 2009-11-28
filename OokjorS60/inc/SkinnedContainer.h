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

//learned from http://wiki.forum.nokia.com/index.php/Drawing_skins_in_container

#ifndef SKINNEDCONTAINER_H
#define SKINNEDCONTAINER_H

#include <coecntrl.h>       // CCoeControl
#include <aknsbasicbackgroundcontrolcontext.h>
#include <aknsdrawutils.h>
#include <aknscontrolcontext.h>
#include <aknsskininstance.h>
#include <aknsutils.h>

class CSkinnedContainer : public CCoeControl
    {
public:
	virtual ~CSkinnedContainer();
protected:
	virtual TTypeUid::Ptr MopSupplyObject(TTypeUid aId); //
	virtual void SizeChanged();
	virtual void HandleResourceChange(TInt aType);
	virtual void ConstructL(void);
	virtual void Draw(const TRect& aRect) const;
	CAknsBasicBackgroundControlContext*	iBgContext;
};

#endif
