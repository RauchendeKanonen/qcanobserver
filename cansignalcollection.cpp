#include "cansignalcollection.h"

CANSignalCollection::CANSignalCollection()
{
    CANSignalList = new QList<CANSignal*>;
}


void CANSignalCollection::addSignal(int AId, float AOffset, float AMultiplier, QString AName, int *AMask, int *AConstrainMask,
		     QString AUnit, bool AisEventItem, long AOnOffConstrain, bool AisSigned, bool AisMaskConstrain, int ASigType)
{
    CANSignalList->append(new CANSignal(AId, AOffset, AMultiplier, AName, AMask, AConstrainMask,
		     AUnit, AisEventItem, AOnOffConstrain, AisSigned, AisMaskConstrain, ASigType));
}


CANSignal* CANSignalCollection::getSignal(int id, QString Name)
{
    const char* p = Name.toStdString().c_str();
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
