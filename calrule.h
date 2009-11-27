#ifndef CALRULE_H
#define CALRULE_H
#include<QString>
#include <QList>

#define MAX_RULES 32

class RuleInformation
{
    public:
    RuleInformation(float AOffset, float AMultiplier, QString AName, int AMask[8], QString AUnit, bool AOnOff)
    {
        Unit = AUnit;
        OnOff = AOnOff;
        Name = AName;
        Offset = AOffset;
        Multiplier = AMultiplier;
        memcpy(Mask, AMask, 8*sizeof(int));
    }
    QString     Unit;
    bool        OnOff;
    int         Mask[8];
    QString     Name;
    float       Offset;
    float       Multiplier;
};

class IDCollection
{
    public:
    QString     Unit;
    bool        OnOff;
    QString     Name;
    float       Value;
};

//!This Class is the Storrage Place for the Calculation Rules.
//!It stores all Rules for one CAN ID. The access is done over a int Index which specifies the Rule

class CalRule
{
    public:
        CalRule(int AID, float AOffset, float AMultiplier, QString AName, int AMask[8], QString AUnit, bool AOnOff);
        QString  getName(int Rule)   const               { return Set->at(Rule)->Name; }
        float getValue(char Data[8], int Rule);
        float getValue(char Data[8], QString RuleName, bool *OK);
        int getIDCollection(char Data[8], QList <IDCollection*> *Col);
        int addRule(float AOffset, float AMultiplier, QString AName, int AMask[8], QString AUnit, bool AOnOff);
        QString getUnit(int Rule);
        int getId(void);
        int getNumOfRules(void);

    private:
        int         ID;
        int         Rules;
        QList <RuleInformation*> *Set;
};
#endif // CALRULE_H
