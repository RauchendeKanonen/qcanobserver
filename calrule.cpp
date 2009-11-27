#include "calrule.h"


//!The Constructor takes all the information for one Rule (Item in the XML Database).
//Additionally it takes the Message ID
//!AMask describes how to put the data for this Rule(Value) together.
//!12003400 as Mask leads to a Value collected of the first two bytes and the fith and sixth.
//!The Bytes are shifted by the Value(in Bytes, not Bits) in the Mask into the Value.
CalRule::CalRule(int AID, float AOffset, float AMultiplier, QString AName, int AMask[8], QString AUnit, bool AOnOff)
{
    Set = new QList <RuleInformation*>;
    Set->insert(0, new RuleInformation(AOffset, AMultiplier, AName, AMask, AUnit, AOnOff));
    ID = AID;
    Rules = 1;
}
//!AddRule takes all the information for one Rule (Item in the XML Database)
//!AMask describes how to put the data for this Rule(Value) together.
//!12003400 as Mask leads to a Value collected of the first two bytes and the fith and sixth.
//!The Bytes are shifted by the Value(in Bytes, not Bits) in the Mask into the Value.
int CalRule::addRule(float AOffset, float AMultiplier, QString AName, int AMask[8], QString AUnit, bool AOnOff)
{
    Set->insert(0, new RuleInformation(AOffset, AMultiplier, AName, AMask, AUnit, AOnOff));
    Rules++;
    return 1;
}
//!returns the number of Rules associated with an ID
int CalRule::getNumOfRules(void)
{
    return Rules;
}

//!Takes the Data of a CAN-Frame with the Rule-Index and calculates the associated Value.
//!AMask describes how to put the data for this Rule(Value) together.
//!12003400 as Mask leads to a Value collected of the first two bytes and the fith and sixth.
//!The Bytes are shifted by the Value(in Bytes, not Bits) in the Mask into the Value.
//!The Value is then multiplied by the specified Multiplier, then the offset is substracted.
float CalRule::getValue(char Data[8], int Rule)
{
    float Ret;
    long Value = 0;

    for( int i = 1 ; i <= 8 ; i++)
    {
        if(Set->at(Rule)->Mask[0] == i)
            Value |= Data[0] << ((i-1)*8);
        if(Set->at(Rule)->Mask[1] == i)
            Value |= Data[1] << ((i-1)*8);
        if(Set->at(Rule)->Mask[2] == i)
            Value |= Data[2] << ((i-1)*8);
        if(Set->at(Rule)->Mask[3] == i)
            Value |= Data[3] << ((i-1)*8);
        if(Set->at(Rule)->Mask[4] == i)
            Value |= Data[4] << ((i-1)*8);
        if(Set->at(Rule)->Mask[5] == i)
            Value |= Data[5] << ((i-1)*8);
        if(Set->at(Rule)->Mask[6] == i)
            Value |= Data[6] << ((i-1)*8);
        if(Set->at(Rule)->Mask[7] == i)
            Value |= Data[7] << ((i-1)*8);

    }
    Ret = Value * Set->at(Rule)->Multiplier - Set->at(Rule)->Offset;
    return Ret;
}
//!returns the ID
int CalRule::getId(void)
{
    return ID;
}
//!Takes the CAN Frame Data, returns the Number of associated Rules and writes
//!a QList <IDCollection*> to Col
//!The Caller is liable for freeing the memory
int CalRule::getIDCollection(char Data[8], QList <IDCollection*> *Col)
{

    for(int i = 0; i < Rules ; i ++ )
    {
        IDCollection *locCol = new IDCollection();
        locCol->Value = getValue(Data,i);
        locCol->Name = getName(i);
        locCol->Unit = getUnit(i);

        Col->append(locCol);
    }
    return Rules;
}

//!Returns the physical Unit for this Rule
QString CalRule::getUnit(int Rule)
{

    return Set->at(Rule)->Unit;
}
