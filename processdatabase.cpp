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
#include "processdatabase.h"
#include <QtXml>

#include <qfile.h>
#include <QMessageBox>




//!Reads all the Items out of the XML File and Creates a QList <CalRule*>
ProcessDataBase::ProcessDataBase(QString FileName)
{
    DBFileName.clear();
    Collection = NULL;
     QFile opmlFile( FileName );
    if ( !opmlFile.open( QIODevice::ReadOnly ) ) {
        QMessageBox::critical( 0,
                QString( "Critical Error" ),
                QString( "Cannot open database file" ));
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

    Collection = new CANSignalCollection();

    DBFileName = FileName;

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
                bool    isEventItem = false;
                QDomNode Interpret_ = Item_.firstChild();
                QDomElement Interpret = Interpret_.toElement();

                QString Name = Interpret.attribute( "Name" );
                QString Mul = Interpret.attribute( "Multiplier" );
                QString Offset = Interpret.attribute( "Offset" );
                QString MaskStr = Interpret.attribute( "Datamask" );
                QString ConstrainMaskStr = Interpret.attribute( "ConstrainMask" );

                QString UnitStr = Interpret.attribute( "Unit" );

                QString EventItem = Interpret.attribute( "EventItem" );
                QString ConstrainValStr = Interpret.attribute( "Constrain" );

                QString SignalTypeStr = Interpret.attribute( "Type" );

                int SignalType = CANSIG_INT;

                if(SignalTypeStr == QString("float"))
                    SignalType = CANSIG_FLOAT;

                if(SignalTypeStr == QString("double"))
                    SignalType = CANSIG_DOUBLE;


                long ConstrainVal = ConstrainValStr.toLong(NULL, 16);
                if(EventItem.compare("true", Qt::CaseSensitive) == 0)
                    isEventItem = true;

                int Mask[8];
                if(MaskStr.count()==8)
                {
                    for(int c = 0 ; c < 8 ; c++ )
                        Mask[c] = atoi((char*)&MaskStr.at(c));
                }

                int ConstrainMask[8];
                if(ConstrainMaskStr.count()==8)
                {
                    for(int c = 0 ; c < 8 ; c++ )
                        ConstrainMask[c] = atoi((char*)&ConstrainMaskStr.at(c));
                }

		Name = QString("(")+Name+QString(")");

		Collection->addSignal(id.toInt(NULL,16),
				      Offset.toFloat(NULL),
				      Mul.toFloat(NULL),
				      Name,
				      Mask,
				      ConstrainMask,
				      UnitStr,
				      isEventItem,
				      ConstrainVal,
				      true,
				      true,
                                      SignalType);



                Item_ = ItemList.at(f);
            }
        }
        node = node.nextSibling();
    }
}

CANSignalCollection * ProcessDataBase::getCANSignalList()
{
    return Collection;
}
