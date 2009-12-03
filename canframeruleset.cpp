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
#include "canframeruleset.h"


//!The Constructor takes all the information for one Rule (Item in the XML Database).
//Additionally it takes the Message ID
//!AMask describes how to put the data for this Rule(Value) together.
//!12003400 as Mask leads to a Value collected of the first two bytes and the fith and sixth.
//!The Bytes are shifted by the Value(in Bytes, not Bits) in the Mask into the Value.
CanFrameRuleSet::CanFrameRuleSet(int AID, float AOffset, float AMultiplier, QString AName, int AMask[8], int AConstrainMask[8], QString AUnit, bool AOnOff, long AOnOffConstrain)
{
    Set = new QList <RuleInformation*>;
    Set->insert(0, new RuleInformation(AOffset, AMultiplier, AName, AMask, AConstrainMask, AUnit, AOnOff, AOnOffConstrain));
    ID = AID;
    Rules = 1;
}
//!AddRule takes all the information for one Rule (Item in the XML Database)
//!AMask describes how to put the data for this Rule(Value) together.
//!12003400 as Mask leads to a Value collected of the first two bytes and the fith and sixth.
//!The Bytes are shifted by the Value(in Bytes, not Bits) in the Mask into the Value.
int CanFrameRuleSet::addRule(float AOffset, float AMultiplier, QString AName, int AMask[8], int AConstrainMask[8], QString AUnit, bool AOnOff, long AOnOffConstrain)
{
    Set->insert(0, new RuleInformation(AOffset, AMultiplier, AName, AMask, AConstrainMask, AUnit, AOnOff, AOnOffConstrain));
    Rules++;
    return 1;
}
//!returns the number of Rules associated with an ID
int CanFrameRuleSet::getNumOfRules(void)
{
    return Rules;
}

//!Takes the Data of a CAN-Frame with the Rule-Index and calculates the associated Value.
//!AMask describes how to put the data for this Rule(Value) together.
//!12003400 as Mask leads to a Value collected of the first two bytes and the fith and sixth.
//!The Bytes are shifted by the Value(in Bytes, not Bits) in the Mask into the Value.
//!The Value is then multiplied by the specified Multiplier, then the offset is substracted.
float CanFrameRuleSet::getValue(unsigned char Data[8], int Rule)
{
    float Ret;
    unsigned long long Value = 0;

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
int CanFrameRuleSet::getId(void)
{
    return ID;
}


//!Takes the Data of a CAN-Frame with the Rule-Index and calculates the associated Value with the ConstrainMask
//!ConstrainMask describes how to put the data for this Rule(Constrain) together.
//!12003400 as Mask leads to a Value collected of the first two bytes and the fith and sixth.
//!The Bytes are shifted by the Value(in Bytes, not Bits) in the Mask into the Value.
//!The Value is then multiplied by the specified Multiplier, then the offset is substracted.
unsigned long long CanFrameRuleSet::getConstrainMaskMaskedData(unsigned char Data[8], int Rule)
{
    unsigned long long Value = 0;


    if(Data[2] != 0)
        Value = 0;


    for( int i = 1 ; i <= 8 ; i++)
    {
        if(Set->at(Rule)->ConstrainMask[0] == i)
            Value |= Data[0] << ((i-1)*8);
        if(Set->at(Rule)->ConstrainMask[1] == i)
            Value |= Data[1] << ((i-1)*8);
        if(Set->at(Rule)->ConstrainMask[2] == i)
            Value |= Data[2] << ((i-1)*8);
        if(Set->at(Rule)->ConstrainMask[3] == i)
            Value |= Data[3] << ((i-1)*8);
        if(Set->at(Rule)->ConstrainMask[4] == i)
            Value |= Data[4] << ((i-1)*8);
        if(Set->at(Rule)->ConstrainMask[5] == i)
            Value |= Data[5] << ((i-1)*8);
        if(Set->at(Rule)->ConstrainMask[6] == i)
            Value |= Data[6] << ((i-1)*8);
        if(Set->at(Rule)->ConstrainMask[7] == i)
            Value |= Data[7] << ((i-1)*8);
    }

  return Value;
}

//!Checks if the event specified in the int ConstrainMask[] occured in AData[]
//!If ConstrainMask only consists of a single bit then the occurence of this bit in the data
//!triggers the event.
//!If ConstrainMask consists of more than one bit an exact match is required to trigger the event
bool CanFrameRuleSet::checkOnOff(unsigned char AData[8], int Rule)
{
    long long Data = getConstrainMaskMaskedData(AData, Rule);

    int bits = 0;


    for(int i = 0 ; i < 64 ; i ++ )
    {
        if(Set->at(Rule)->OnOffConstrain & (((long long)1)<<i))
            bits++;
    }

    //Only one bit in the Mask, and we return if the bit was set in the data
    if(bits == 1)
    {
        if((Data & Set->at(Rule)->OnOffConstrain) == Set->at(Rule)->OnOffConstrain)
            return true;
        else
            return false;
    }
    //More than one bit and we return an exact match
    else
    {
       if((Data == Set->at(Rule)->OnOffConstrain))
            return true;
        else
            return false;
    }

    return false;

}

//!Takes the CAN Frame Data, returns the Number of associated Rules and writes
//!a QList <IDCollection*> to Col
//!The Caller is liable for freeing the memory
int CanFrameRuleSet::getIDCollection(unsigned char Data[8], QList <IDCollection*> *Col)
{

    for(int i = 0; i < Rules ; i ++ )
    {
        if(Set->at(i)->isEventItem == true)
        {
            // has the event happened
            if(checkOnOff(Data, i))
            {
                 IDCollection *locCol = new IDCollection();
                 locCol->Name = QString("Evt: ")+getName(i);
                 locCol->Value = getValue(Data,i);
                 locCol->Unit = getUnit(i);
                 locCol->isEventItem = true;
                 locCol->Rule = i;
                 Col->append(locCol);
            }
            continue;
        }
        IDCollection *locCol = new IDCollection();
        locCol->Value = getValue(Data,i);
        locCol->Name = getName(i);
        locCol->Unit = getUnit(i);
        locCol->isEventItem = false;
        locCol->Rule = i;
        Col->append(locCol);
    }
    return Rules;
}

//!Returns the physical Unit for this Rule
QString CanFrameRuleSet::getUnit(int Rule)
{

    return Set->at(Rule)->Unit;
}
