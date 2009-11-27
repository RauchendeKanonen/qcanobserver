#ifndef PROCESSDATABASE_H
#define PROCESSDATABASE_H
#include <QString>
#include <qdom.h>
#include <QList>
#include "calrule.h"



class ProcessDataBase
{
public:
    ProcessDataBase(QString FileName);
    void getRuleList(QList<CalRule*> **lst);
private:
    int getValueNamePairs(char Data[8], int id, float *Value, QString *Name);
    int getNumOfValueNamePairs(int id);
    QDomDocument domTree;
    QList<CalRule*> list;
    CalRule* findId(int id);

};

#endif // PROCESSDATABASE_H
