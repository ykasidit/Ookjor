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


#ifndef __Ookjor_APPLICATION_H__
#define __Ookjor_APPLICATION_H__

#include <aknapp.h>


/*!
  @class COokjorApplication

  @discussion An instance of COokjorApplication is the application part of the AVKON
  application framework for the Ookjor example application
  */
class COokjorApplication : public CAknApplication
    {
public:  // from CAknApplication

/*!
  @function AppDllUid

  @discussion Returns the application DLL UID value
  @result the UID of this Application/Dll
  */
    TUid AppDllUid() const;

protected: // from CAknApplication
/*!
  @function CreateDocumentL

  @discussion Create a CApaDocument object and return a pointer to it
  @result a pointer to the created document
  */
    CApaDocument* CreateDocumentL();
    };

#ifdef EKA2
    static const TUid KUidOokjor = {0xA000EC71};
#else
    static const TUid KUidOokjor = {0x2001A3E4};
#endif

#endif // __Ookjor_APPLICATION_H__
