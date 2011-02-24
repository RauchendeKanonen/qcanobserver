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
#include<QString>
#ifndef CANSIGNAL_H
#define CANSIGNAL_H


#define CANSIG_FLOAT  1
#define CANSIG_INT    2     //1/2/4/8Byte
#define CANSIG_DOUBLE 3



class SignalDataCollection
{
    public:
    QString     Unit;
    bool        isEventItem;
    QString     Name;
    double long       Value;
};


class CANSignal
{
public:
    CANSignal(int AId, float AOffset, float AMultiplier, QString AName, int *AMask, int *AConstrainMask,
	      QString AUnit, bool AisEventItem, long AOnOffConstrain, bool AisSigned, bool AisMaskConstrain, int ASigType);
    float getValue(unsigned char Data[8]);
    unsigned long long getConstrainMaskMaskedData(unsigned char Data[8]);
    bool checkOnOff(unsigned char AData[8]);
    bool getSignalDataCollection(unsigned char Data[8],  SignalDataCollection *SigCol);
    float getInteger(unsigned char Data[8]);
    float getFloat(unsigned char Data[8]);

    char* CreateDataFromValue(float Value);
    char* CreateDataFromFloat(float Value);
    char* CreateDataFromInteger(float ValueFl);

    ~CANSignal()
    {
    }
    int		Id;
    QString     Unit;
    bool        isEventItem;
    QString     Name;
private:
    bool        isSigned;
    int         SigType;
    int         Mask[8];
    int         ConstrainMask[8];
    float       Offset;
    float       Multiplier;
    long long   OnOffConstrain;
};

#endif // CANSIGNALCOLLECTION_H
