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
#include "cansignal.h"

CANSignal::CANSignal(int AId, float AOffset, float AMultiplier, QString AName, int *AMask, int *AConstrainMask,
		     QString AUnit, bool AisEventItem, long AOnOffConstrain, bool AisSigned, bool AisMaskConstrain, int ASigType)
{
	Id = AId;
	Unit = AUnit;
	isEventItem = AisEventItem;
	Name = AName;
	Offset = AOffset;
	Multiplier = AMultiplier;
	memcpy(Mask, AMask, 8*sizeof(int));
	OnOffConstrain = AOnOffConstrain;
	memcpy(ConstrainMask, AConstrainMask, 8*sizeof(int));
        SigType = ASigType;
        isSigned = AisSigned;
}

float CANSignal::getValue(unsigned char Data[8])
{
    float ret;

    if(SigType == CANSIG_INT)
        ret = getInteger(Data);
    if(SigType == CANSIG_FLOAT)
        ret = getFloat(Data);


    return ret;
}

float CANSignal::getFloat(unsigned char Data[8])
{
    float Ret;
    int Value = 0;
    char msb;
    int lastshift;
    int num_of_bytes=0;

    for( int i = 1 ; i <= 8 ; i++)
    {
        if(Mask[0] == i)
        {
            num_of_bytes ++;
            lastshift = i;
            msb = Data[0];
            Value |= Data[0] << ((i-1)*8);
        }
        if(Mask[1] == i)
        {
            num_of_bytes ++;
            lastshift = i;
            msb = Data[1];
            Value |= Data[1] << ((i-1)*8);
        }
        if(Mask[2] == i)
        {
            num_of_bytes ++;
            lastshift = i;
            msb = Data[2];
            Value |= Data[2] << ((i-1)*8);
        }
        if(Mask[3] == i)
        {
            num_of_bytes ++;
            lastshift = i;
            msb = Data[3];
            Value |= Data[3] << ((i-1)*8);
        }
        if(Mask[4] == i)
        {
            num_of_bytes ++;
            lastshift = i;
            msb = Data[4];
            Value |= Data[4] << ((i-1)*8);
        }
        if(Mask[5] == i)
        {
            num_of_bytes ++;
            lastshift = i;
            msb = Data[5];
            Value |= Data[5] << ((i-1)*8);
        }
        if(Mask[6] == i)
        {
            num_of_bytes ++;
            lastshift = i;
            msb = Data[6];
            Value |= Data[6] << ((i-1)*8);
        }
        if(Mask[7] == i)
        {
            num_of_bytes ++;
            lastshift = i;
            msb = Data[7];
            Value |= Data[7] << ((i-1)*8);
        }

    }

    if(num_of_bytes == 4)
    {
        memcpy(&Ret, (&Value), 4);
        Ret *= Multiplier - Offset;
    }

    if(isSigned == false && Ret < 0)
        Ret *= -1.0;

    return Ret;
}

char* CANSignal::CreateDataFromValue(float Value)
{

}

char* CANSignal::CreateDataFromInteger(float ValueFl)
{
    char Data[8];
    int Value = (int) ValueFl;

    for( int i = 1 ; i <= 8 ; i++)
    {
        if(Mask[0] == i)
            Data[0] = (char)(Value >> ((i-1)*8));

        if(Mask[1] == i)
            Data[1] = (char)(Value >> ((i-1)*8));

        if(Mask[2] == i)
            Data[2] = (char)(Value >> ((i-1)*8));

        if(Mask[3] == i)
            Data[3] = (char)(Value >> ((i-1)*8));

        if(Mask[4] == i)
            Data[4] = (char)(Value >> ((i-1)*8));

        if(Mask[5] == i)
            Data[5] = (char)(Value >> ((i-1)*8));

        if(Mask[6] == i)
            Data[6] = (char)(Value >> ((i-1)*8));

        if(Mask[7] == i)
            Data[7] = (char)(Value >> ((i-1)*8));
    }

    return Data;
}


char* CANSignal::CreateDataFromFloat(float ValueFl)
{
    char Data[8];
    int Value;
    memcpy(&Value, &ValueFl, 4);
    int num_of_bytes=0;

    for( int i = 1 ; i <= 8 ; i++)
    {
        if(Mask[0] == i)
            Data[0] = (char)(Value >> ((i-1)*8));

        if(Mask[1] == i)
            Data[1] = (char)(Value >> ((i-1)*8));

        if(Mask[2] == i)
            Data[2] = (char)(Value >> ((i-1)*8));

        if(Mask[3] == i)
            Data[3] = (char)(Value >> ((i-1)*8));

        if(Mask[4] == i)
            Data[4] = (char)(Value >> ((i-1)*8));

        if(Mask[5] == i)
            Data[5] = (char)(Value >> ((i-1)*8));

        if(Mask[6] == i)
            Data[6] = (char)(Value >> ((i-1)*8));

        if(Mask[7] == i)
            Data[7] = (char)(Value >> ((i-1)*8));
    }

    return Data;
}

//!Takes the Data of a CAN-Frame with the Rule-Index and calculates the associated Value.
//!AMask describes how to put the data for this Rule(Value) together.
//!12003400 as Mask leads to a Value collected of the first two bytes and the fith and sixth.
//!The Bytes are shifted by the Value(in Bytes, not Bits) in the Mask into the Value.
//!The Value is then multiplied by the specified Multiplier, then the offset is substracted.
float CANSignal::getInteger(unsigned char Data[8])
{
    float Ret;
    long long Value = 0;
    char msb;
    int lastshift;
    int num_of_bytes=0;

    for( int i = 1 ; i <= 8 ; i++)
    {
	if(Mask[0] == i)
        {
            num_of_bytes ++;
            lastshift = i;
            msb = Data[0];
	    Value |= Data[0] << ((i-1)*8);
        }
	if(Mask[1] == i)
        {
            num_of_bytes ++;
            lastshift = i;
            msb = Data[1];
	    Value |= Data[1] << ((i-1)*8);
        }
	if(Mask[2] == i)
        {
            num_of_bytes ++;
            lastshift = i;
            msb = Data[2];
	    Value |= Data[2] << ((i-1)*8);
        }
	if(Mask[3] == i)
        {
            num_of_bytes ++;
            lastshift = i;
            msb = Data[3];
	    Value |= Data[3] << ((i-1)*8);
        }
        if(Mask[4] == i)
        {
            num_of_bytes ++;
            lastshift = i;
            msb = Data[4];
	    Value |= Data[4] << ((i-1)*8);
        }
	if(Mask[5] == i)
        {
            num_of_bytes ++;
            lastshift = i;
            msb = Data[5];
	    Value |= Data[5] << ((i-1)*8);
        }
	if(Mask[6] == i)
        {
            num_of_bytes ++;
            lastshift = i;
            msb = Data[6];
	    Value |= Data[6] << ((i-1)*8);
        }
	if(Mask[7] == i)
        {
            num_of_bytes ++;
            lastshift = i;
            msb = Data[7];
	    Value |= Data[7] << ((i-1)*8);
        }

    }

    if(num_of_bytes == 1)
    {
        if(isSigned == false)
        {
            unsigned char datum = Value;
            Ret = datum * Multiplier - Offset;
        }

        if(isSigned == true)
        {
            char datum = Value;
            Ret = datum * Multiplier - Offset;
        }
    }

    if(num_of_bytes == 2)
    {
        if(isSigned == false)
        {
            unsigned short datum = Value;
            Ret = datum * Multiplier - Offset;
        }

        if(isSigned == true)
        {
            short datum = Value;
            Ret = datum * Multiplier - Offset;
        }
    }

    if(num_of_bytes == 4)
    {
        if(isSigned == false)
        {
            unsigned int datum = Value;
            Ret = datum * Multiplier - Offset;
        }

        if(isSigned == true)
        {
            int datum = Value;
            Ret = datum * Multiplier - Offset;
        }
    }

    if(num_of_bytes == 8)
    {
        if(isSigned == false)
        {
            unsigned long long datum = Value;
            Ret = datum * Multiplier - Offset;
        }

        if(isSigned == true)
        {
            long long datum = Value;
            Ret = datum * Multiplier - Offset;
        }
    }

    return Ret;
}

//!Takes the Data of a CAN-Frame with the Rule-Index and calculates the associated Value with the ConstrainMask
//!ConstrainMask describes how to put the data for this Rule(Constrain) together.
//!12003400 as Mask leads to a Value collected of the first two bytes and the fith and sixth.
//!The Bytes are shifted by the Value(in Bytes, not Bits) in the Mask into the Value.
//!The Value is then multiplied by the specified Multiplier, then the offset is substracted.
unsigned long long CANSignal::getConstrainMaskMaskedData(unsigned char Data[8])
{
    unsigned long long Value = 0;


    for( int i = 1 ; i <= 8 ; i++)
    {
	if(ConstrainMask[0] == i)
	    Value |= Data[0] << ((i-1)*8);
	if(ConstrainMask[1] == i)
	    Value |= Data[1] << ((i-1)*8);
	if(ConstrainMask[2] == i)
	    Value |= Data[2] << ((i-1)*8);
	if(ConstrainMask[3] == i)
	    Value |= Data[3] << ((i-1)*8);
	if(ConstrainMask[4] == i)
	    Value |= Data[4] << ((i-1)*8);
	if(ConstrainMask[5] == i)
	    Value |= Data[5] << ((i-1)*8);
	if(ConstrainMask[6] == i)
	    Value |= Data[6] << ((i-1)*8);
	if(ConstrainMask[7] == i)
	    Value |= Data[7] << ((i-1)*8);
    }

  return Value;
}

//!Checks if the event specified in the int ConstrainMask[] occured in AData[]
//!If ConstrainMask only consists of a single bit then the occurence of this bit in the data
//!triggers the event.
//!If ConstrainMask consists of more than one bit an exact match is required to trigger the event
bool CANSignal::checkOnOff(unsigned char AData[8])
{
    unsigned long long Data = getConstrainMaskMaskedData(AData);

    int bits = 0;


    for(int i = 0 ; i < 64 ; i ++ )
    {
	if(OnOffConstrain & (((long long)1)<<i))
	    bits++;
    }

    //Only one bit in the Mask, and we return if the bit was set in the data
    if(bits == 1)
    {
	if((Data & OnOffConstrain) == OnOffConstrain)
	    return true;
	else
	    return false;
    }
    //More than one bit and we return an exact match
    else
    {
       if((Data == OnOffConstrain))
	    return true;
	else
	    return false;
    }

    return false;

}

//!Takes the CAN Frame Data, returns the Number of associated Rules and writes
//!a QList <IDCollection*> to Col
//!The Caller is liable for freeing the memory
bool CANSignal::getSignalDataCollection(unsigned char Data[8], SignalDataCollection *SigCol)
{

    if(isEventItem == true)
    {
        // has the event happened
        if(checkOnOff(Data))
        {
            SigCol->Name = QString("Evt: ")+Name;
            SigCol->Value = getValue(Data);
            SigCol->Unit = Unit;
            SigCol->isEventItem = true;
            return true;
        }
        return false;
    }
    SigCol->Value = getValue(Data);
    SigCol->Name = Name;
    SigCol->Unit = Unit;
    SigCol->isEventItem = false;
    return true;
}
