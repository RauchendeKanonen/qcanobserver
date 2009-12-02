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
#ifndef CALRULE_H
#define CALRULE_H
#include<QString>
#include <QList>

#define MAX_RULES 32

class RuleInformation
{
    public:
    RuleInformation(float AOffset, float AMultiplier, QString AName, int AMask[8], QString AUnit, bool AisEventItem, long AOnOffConstrain)
    {
        Unit = AUnit;
        isEventItem = AisEventItem;
        Name = AName;
        Offset = AOffset;
        Multiplier = AMultiplier;
        memcpy(Mask, AMask, 8*sizeof(int));
        OnOffConstrain = AOnOffConstrain;
    }

    QString     Unit;
    bool        isEventItem;
    int         Mask[8];
    QString     Name;
    float       Offset;
    float       Multiplier;
    long        OnOffConstrain;

};

class IDCollection
{
    public:
    QString     Unit;
    bool        isEventItem;
    QString     Name;
    float       Value;
    int         Rule;
};

//!This Class is the Storrage Place for the Calculation Rules.
//!It stores all Rules for one CAN ID. The access is done over a int Index which specifies the Rule

class CanFrameRuleSet
{
    public:
        CanFrameRuleSet(int AID, float AOffset, float AMultiplier, QString AName, int AMask[8], QString AUnit, bool AisEventItem, long AOnOffConstrain);
        QString  getName(int Rule)   const               { return Set->at(Rule)->Name; }
        float getValue(char Data[8], int Rule);
        float getValue(char Data[8], QString RuleName, bool *OK);
        int getIDCollection(char Data[8], QList <IDCollection*> *Col);
        int addRule(float AOffset, float AMultiplier, QString AName, int AMask[8], QString AUnit, bool AisEventItem, long AOnOffConstrain);
        QString getUnit(int Rule);
        int getId(void);
        int getNumOfRules(void);
        bool checkOnOff(char Data[8], int Rule);
        unsigned long getMaskedData(unsigned char Data[8], int Rule);

    private:
        int         ID;
        int         Rules;
        QList <RuleInformation*> *Set;
};
#endif // CALRULE_H
