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
#include "graphicwindow.h"
#include "ui_graphicwindow.h"


GraphicWindow::GraphicWindow(QWidget *parent, CANSignalCollection *Collection) :
    QWidget(parent),
    m_ui(new Ui::GraphicWindow)
{
    pCollection = Collection;

    m_ui->setupUi(this);
    Plot = new QCanPlot(QString(""), this);
    m_ui->verticalLayout->addWidget(Plot);

    Plot->show();

    Plot->setAxisTitle(QwtPlot::xBottom,"Time s");
    Plot->setAxisAutoScale(QwtPlot::xBottom);

    Sel = new CANDataItemSelector(NULL, pCollection);
    connect(Sel, SIGNAL(addItemToDraw(CANSignal*, QColor)), this, SLOT(addItemToDraw(CANSignal*, QColor)));
    connect(Sel, SIGNAL(deleteItemToDraw(CANSignal*)), this, SLOT(deleteItemToDraw(CANSignal*)));
}

GraphicWindow::~GraphicWindow()
{
    delete m_ui;
    delete Sel;
}

void GraphicWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void GraphicWindow::newMessage(CANMsgandTimeStruct *Msg, int Cnt)
{
    int i;
    for(i = 0 ; Curves.count() >  i ; i++)
    {
	if(Msg->CANMsg.ID == Curves.at(i)->pSignal->Id)
        {
	    SignalDataCollection *DataCol = Curves.at(i)->pSignal->getSignalDataCollection(Msg->CANMsg.DATA);

	    if(DataCol)
	    {
		Curves.at(i)->y.append(DataCol->Value);
		Curves.at(i)->x.append((double)Msg->timev.tv_sec + (double)Msg->timev.tv_usec/1000000.0);
                //Plot->setAxisScale(QwtPlot::xBottom, Curves.at(i)->x.last()-10,Curves.at(i)->x.last(), 10);
	    }
	}
    }

}

void GraphicWindow::MainTimerSlot()
{
       for(int i = 0 ; Curves.count() >  i ; i++)
       {
           if(Curves.at(i)->x.count())
           {
                Curves.at(i)->PlotCurve->setRawData((double*)&Curves.at(i)->x.first(),(double*)&Curves.at(i)->y.first(), Curves.at(i)->x.size());
                Curves.at(i)->PlotCurve->attach(Plot);

           }
       }
       Plot->replot();

}

void GraphicWindow::ClearAll()
{
       for(int i = 0 ; Curves.count() >  i ; i++)
       {
           if(Curves.at(i)->x.count())
           {
                Curves.at(i)->PlotCurve->setRawData((double*)&Curves.at(i)->x.first(),(double*)&Curves.at(i)->y.first(), 0);
                Curves.at(i)->y.clear();
                Curves.at(i)->x.clear();
           }

       }
      Plot->replot();
}


void GraphicWindow::on_GraphFromScratch_clicked()
{

}

void GraphicWindow::on_GraphFromDB_clicked()
{
    Sel->show();
}


//!SLOT that addes an Item to draw
//!Takes a CalRule (specific for an ID) and the invoked Rule
void GraphicWindow::addItemToDraw(CANSignal* Signal, QColor Color)
{
    QBrush br(Qt::NoBrush);
    br.setColor(Color);
    QPen Pen(Color);
    Curves.append(new ItemCurveInfo(Signal));
    Curves.last()->PlotCurve->setBrush(br);
    Curves.last()->PlotCurve->setPen(Pen);
}

void GraphicWindow::deleteItemToDraw(CANSignal* Signal)
{
    for(int i = 0 ; i < Curves.count() ; i++ )
    {
	if(Curves.at(i)->pSignal == Signal)
        {
            Curves.at(i)->PlotCurve->detach();
            Curves.removeAt(i);
        }
    }

    Plot->replot();
}
void GraphicWindow::StopCapture()
{
    Plot->setAutoScaleCanvas();
}
