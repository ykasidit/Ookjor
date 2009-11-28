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


// INCLUDE FILES

// Class include
#include "SettingsListSettings.h"

// ================= MEMBER FUNCTIONS =======================



TSettingsListSettings::TSettingsListSettings()
{

	iCycleTime = 1;
	iStartMinute =1;
	iPreCycleTime = 10;
}


void TSettingsListSettings::ExternalizeL(RWriteStream &stream)
	{
		/*
		TUint8 version  : x.y = x*10+y so version 1.0 is 10
		TUint8 iCycleTime;
		TUint8 iStartMinute;
		TBool iPreCycleAlertEnabled;
		TUint8 iPreCycleTime;
		*/


		stream.WriteUint8L(iCycleTime);
		stream.WriteUint8L(iStartMinute);
		stream.WriteUint8L(iPreCycleTime);
		stream.CommitL();

	}
void TSettingsListSettings::InternalizeL(RReadStream &stream)
	{


		iCycleTime=stream.ReadUint8L();
		iStartMinute=stream.ReadUint8L();
		iPreCycleTime =stream.ReadUint8L();
	}




