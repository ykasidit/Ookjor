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


#include "OokjorAppui.h"
#include "OokjorDocument.h"

// Standard Symbian OS construction sequence
COokjorDocument* COokjorDocument::NewL(CEikApplication& aApp)
    {
    COokjorDocument* self = NewLC(aApp);
    CleanupStack::Pop(self);
    return self;
    }

COokjorDocument* COokjorDocument::NewLC(CEikApplication& aApp)
    {
    COokjorDocument* self = new (ELeave) COokjorDocument(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

void COokjorDocument::ConstructL()
    {
	// no implementation required
    }

COokjorDocument::COokjorDocument(CEikApplication& aApp) : CAknDocument(aApp)
    {
	// no implementation required
    }

COokjorDocument::~COokjorDocument()
    {
	// no implementation required
    }

CEikAppUi* COokjorDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it,
    // the framework takes ownership of this object
    CEikAppUi* appUi = new (ELeave) COokjorAppUi;
    return appUi;
    }

