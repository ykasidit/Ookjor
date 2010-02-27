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

#ifndef __Ookjor_DOCUMENT_H__
#define __Ookjor_DOCUMENT_H__


#include <akndoc.h>

// Forward references
class COokjorAppUi;
class CEikApplication;


/*!
  @class COokjorDocument

  @discussion An instance of class COokjorDocument is the Document part of the AVKON
  application framework for the Ookjor example application
  */
class COokjorDocument : public CAknDocument
    {
public:

/*!
  @function NewL

  @discussion Construct a COokjorDocument for the AVKON application aApp
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of COokjorDocument
  */
    static COokjorDocument* NewL(CEikApplication& aApp);

/*!
  @function NewLC

  @discussion Construct a COokjorDocument for the AVKON application aApp
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of COokjorDocument
  */
    static COokjorDocument* NewLC(CEikApplication& aApp);

/*!
  @function ~COokjorDocument

  @discussion Destroy the object and release all memory objects
  */
    ~COokjorDocument();

public: // from CAknDocument
/*!
  @function CreateAppUiL

  @discussion Create a COokjorAppUi object and return a pointer to it
  @result a pointer to the created instance of the AppUi created
  */
    CEikAppUi* CreateAppUiL();

private:

/*!
  @function ConstructL

  @discussion Perform the second phase construction of a COokjorDocument object
  */
    void ConstructL();

/*!
  @function COokjorDocument

  @discussion Perform the first phase of two phase construction
  @param aApp application creating this document
  */
    COokjorDocument(CEikApplication& aApp);

    };


#endif // __Ookjor_DOCUMENT_H__
