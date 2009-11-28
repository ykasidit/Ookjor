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

#ifndef HelpTextContainer_H
#define HelpTextContainer_H



// System includes
#include <coecntrl.h> // CCoeControl
#include <txtfrmat.h> // TCharFormatMask
#include "ScrollRichTextContainer.h"

class CMyRichTextEditor;


class CHelpTextContainer : public  CScrollRichTextContainer
	{
public: // Constructors and destructor

	static CHelpTextContainer* NewL(const TRect& aRect,MObjectProvider *provider);
	static CHelpTextContainer* NewLC(const TRect& aRect,MObjectProvider *provider);
	CHelpTextContainer(MObjectProvider *provider);

	void AddTextToEditorL();
	};

#endif
