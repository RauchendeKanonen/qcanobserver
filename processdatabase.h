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
#ifndef PROCESSDATABASE_H
#define PROCESSDATABASE_H
#include <QString>
#include <qdom.h>
#include <QList>
#include "canframeruleset.h"



class ProcessDataBase
{
public:
    ProcessDataBase(QString FileName);
    void getRuleList(QList<CanFrameRuleSet*> **lst);
private:
    int getValueNamePairs(char Data[8], int id, float *Value, QString *Name);
    int getNumOfValueNamePairs(int id);
    QDomDocument domTree;
    QList<CanFrameRuleSet*> list;
    CanFrameRuleSet* findId(int id);

};

#endif // PROCESSDATABASE_H
