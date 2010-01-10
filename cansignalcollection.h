#include <QString>
#include <QList>
#include "cansignal.h"


#ifndef CANSIGNALCOLLECTION_H
#define CANSIGNALCOLLECTION_H

class CANSignalCollection
{
public:
    CANSignalCollection();
    ~CANSignalCollection();

    void addSignal(int AId, float AOffset, float AMultiplier, QString AName, int *AMask, int *AConstrainMask,
                   QString AUnit, bool AisEventItem, long AOnOffConstrain, bool AisSigned, bool AisMaskConstrain, int ASigType);
    CANSignal* getSignal(int id, QString Name);
    CANSignal* getSignal(int index);
    int count(void);
private:
    QList <CANSignal*> *CANSignalList;
};

#endif // CANSIGNALCOLLECTION_H
