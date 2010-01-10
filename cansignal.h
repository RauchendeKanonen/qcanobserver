#include<QString>
#ifndef CANSIGNAL_H
#define CANSIGNAL_H


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
    ~CANSignal()
    {
    }
    int		Id;
    QString     Unit;
    bool        isEventItem;
    QString     Name;
private:


    int         Mask[8];
    int         ConstrainMask[8];
    float       Offset;
    float       Multiplier;
    long long   OnOffConstrain;
};

#endif // CANSIGNALCOLLECTION_H
