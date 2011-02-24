/*
    QCANObserver - A CAN Sniffer
    Copyright (C) <2009>  <Netseal>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "cansignalcollection.h"

CANSignalCollection::CANSignalCollection()
{
    CANSignalList = new QList<CANSignal*>;
}

CANSignalCollection::~CANSignalCollection()
{
    for(int i = 0; i < CANSignalList->count() ; i++)
        delete CANSignalList->at(i);


    delete CANSignalList;
}


void CANSignalCollection::addSignal(int AId, float AOffset, float AMultiplier, QString AName, int *AMask, int *AConstrainMask,
		     QString AUnit, bool AisEventItem, long AOnOffConstrain, bool AisSigned, bool AisMaskConstrain, int ASigType)
{
    CANSignalList->append(new CANSignal(AId, AOffset, AMultiplier, AName, AMask, AConstrainMask,
		     AUnit, AisEventItem, AOnOffConstrain, AisSigned, AisMaskConstrain, ASigType));
}



CANSignal* CANSignalCollection::getSignal(int id, QString Name)
{
    for(int i = 0; i < CANSignalList->count() ; i++)
	if(CANSignalList->at(i)->Name == Name && CANSignalList->at(i)->Id == id)
	    return CANSignalList->at(i);

    return NULL;
}

CANSignal* CANSignalCollection::getSignal(int index)
{
    if(index < CANSignalList->count())
	    return CANSignalList->at(index);

    return NULL;
}

int CANSignalCollection::count(void)
{
    return CANSignalList->count();
}
