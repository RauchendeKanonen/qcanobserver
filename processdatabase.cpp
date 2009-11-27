#include "processdatabase.h"
#include <QtXml>

#include <qfile.h>
#include <QMessageBox>

//!Reads all the Items out of the XML File and Creates a QList <CalRule*>
ProcessDataBase::ProcessDataBase(QString FileName)
{
     QFile opmlFile( FileName );
    if ( !opmlFile.open( QIODevice::ReadOnly ) ) {
        QMessageBox::critical( 0,
                QString( "Critical Error" ),
                QString( "Cannot open file" ));
        return;
    }

    if ( !domTree.setContent( &opmlFile ) ) {
        QMessageBox::critical( 0,
                QString( "Critical Error" ),
                QString( "Parsing error for file" ) );
        opmlFile.close();
        return;
    }
    opmlFile.close();

    // get the header information from the DOM
    QDomElement root = domTree.documentElement();
    QDomNode node;
    node = root.firstChild();
    while ( !node.isNull() )
    {
        if ( node.isElement() && node.nodeName() == "ID" )
        {
            QDomNodeList ItemList = node.childNodes();//.firstChild();
            QDomNode Item_ = ItemList.at(0);
            QDomElement Rule = node.toElement();
            QString id = Rule.attribute( "Id" );
            for(int f = 1 ; !Item_.isNull() ; f ++ )
            {
                QDomNode Interpret_ = Item_.firstChild();
                QDomElement Interpret = Interpret_.toElement();

                QString Name = Interpret.attribute( "Name" );
                QString Mul = Interpret.attribute( "Multiplier" );
                QString Offset = Interpret.attribute( "Offset" );
                QString MaskStr = Interpret.attribute( "Datamask" );
                QString UnitStr = Interpret.attribute( "Unit" );
                QString OnOffStr = Interpret.attribute( "OnOff" );
                int Mask[8];
                for(int c = 0 ; c < 8 ; c++ )
                    Mask[c] = atoi((char*)&MaskStr.at(c));

                CalRule *rule = findId(id.toInt(NULL,16));
                if(rule != NULL)
                    rule->addRule(/*id.toInt(NULL,16),*/ Offset.toFloat(NULL), Mul.toFloat(NULL), Name, Mask, UnitStr,false);
                else
                    list.append(new CalRule(id.toInt(NULL,16), Offset.toFloat(NULL), Mul.toFloat(NULL), Name, Mask, UnitStr,false));

                Item_ = ItemList.at(f);
            }
        }
        node = node.nextSibling();
    }
}
void ProcessDataBase::getRuleList(QList<CalRule*> **lst)
{
    list.count();
    *lst = &list;
}

CalRule* ProcessDataBase::findId(int id)
{
    int count = list.count();

    for(int i = 0 ; i < count ; i ++ )
    {
        if(list.at(i)->getId() == id)
            return list.at(i);
    }
    return NULL;
}
int ProcessDataBase::getNumOfValueNamePairs(int id)
{
    int count = list.count();

    for(int i = 0 ; i < count ; i ++ )
    {
        if(list.at(i)->getId() == id)
            return list.at(i)->getNumOfRules();
    }
    return -1;
}
int ProcessDataBase::getValueNamePairs(char Data[8], int id, float *Value, QString *Name)
{
    int count = list.count();
    CalRule *Cal;

    for(int i = 0 ; 1 ; i ++ )
    {
        if(i == count)
            return -1;

        if(list.at(i)->getId() == id)
        {
            Cal = list.at(i);
            break;
        }
    }

    for(int i = 0; i < Cal->getNumOfRules() ; i++)
    {
        *Name = Cal->getName(i);
        *Value = Cal->getValue(Data, i);
        Name++;
        Value++;
    }
    return count;
}
