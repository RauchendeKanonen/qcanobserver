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
#include "config.h"
#include "graphicwindow.h"
#include "ui_graphicwindow.h"
#include "extrect.h"


GraphicWindow::GraphicWindow(QWidget *parent, CANSignalCollection *Collection) :
    QWidget(NULL),
    m_ui(new Ui::GraphicWindow)
{
    pCollection = Collection;
    pparent = parent;
    m_ui->setupUi(this);
    Plot = new QCanPlot(QString(""), this);
    m_ui->verticalLayout->addWidget(Plot);

    Plot->show();

    Plot->setAxisTitle(QwtPlot::xBottom,"Time s");
    Plot->setAxisAutoScale(QwtPlot::xBottom);

    Sel = new SignalSelectorDialog(NULL, pCollection);
    connect(Sel, SIGNAL(addItemToDraw(CANSignal*, QColor)), this, SLOT(addItemToDraw(CANSignal*, QColor)));
    connect(Sel, SIGNAL(deleteItemToDraw(CANSignal*)), this, SLOT(deleteItemToDraw(CANSignal*)));

    Follow = false;
    FollowTime = 10; //s
}

GraphicWindow::~GraphicWindow()
{


    for(int i = 0; i < Curves.count() ; i++)
        delete Curves.at(i);

    delete Plot;
    delete m_ui;
    delete Sel;
}
void GraphicWindow::closeEvent( QCloseEvent *e )
{
    delete this;
    QWidget::closeEvent( e );
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

void GraphicWindow::newMessage(_CANMsg *CANMsg, int Cnt)
{
    int i;
    for(i = 0 ; Curves.count() >  i ; i++)
    {
        if(CANMsg->ID == (DWORD)Curves.at(i)->pSignal->Id)
        {
            SignalDataCollection DataCol;

            if(Curves.at(i)->pSignal->getSignalDataCollection(CANMsg->DATA, &DataCol))
	    {
                Curves.at(i)->y.append(DataCol.Value);
                Curves.at(i)->x.append((double)CANMsg->tv.tv_sec + (double)CANMsg->tv.tv_usec/1000000.0);


                if(Follow)
                    Plot->setAxisScale(QwtPlot::xBottom, Curves.at(i)->x.last()-FollowTime,Curves.at(i)->x.last()+FollowTime/10, 10);
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
//!Takes a CANSignal (specific for an ID) and the invoked Rule
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
            delete Curves.at(i);
            Curves.removeAt(i);
        }
    }

    Plot->replot();
}
void GraphicWindow::StopCapture()
{
    Plot->setAutoScaleCanvas();
}


//follow
void GraphicWindow::on_FollowCheckBox_toggled(bool checked)
{
#if QWT_VERSION < 0x050200
    if(checked)
    {
        //disable the autoscale checkbox
        m_ui->AutoScalecheckBox->setEnabled(false);
        Follow = true;
        FollowTime = Plot->axisScaleDiv(QwtPlot::xBottom)->hBound() - Plot->axisScaleDiv(QwtPlot::xBottom)->lBound();
    }

    else
    {
        //enable the autoscale checkbox
        m_ui->AutoScalecheckBox->setEnabled(true);

        Follow = false;

        int lowerBound = Plot->axisScaleDiv(QwtPlot::xBottom)->lBound();
        int higherBound = Plot->axisScaleDiv(QwtPlot::xBottom)->hBound();
        //store the autoscaled canvas for max outter zoom
        Plot->setAutoScaleCanvas();
        Plot->setAxisScale(QwtPlot::xBottom, lowerBound, higherBound, 10);
    }
#else
    if(checked)
    {
        //disable the autoscale checkbox
        m_ui->AutoScalecheckBox->setEnabled(false);
        Follow = true;
        FollowTime = Plot->axisScaleDiv(QwtPlot::xBottom)->upperBound() - Plot->axisScaleDiv(QwtPlot::xBottom)->lowerBound();
    }

    else
    {
        //enable the autoscale checkbox
        m_ui->AutoScalecheckBox->setEnabled(true);

        Follow = false;

        int lowerBound = Plot->axisScaleDiv(QwtPlot::xBottom)->lowerBound();
        int higherBound = Plot->axisScaleDiv(QwtPlot::xBottom)->upperBound();
        //store the autoscaled canvas for max outter zoom
        Plot->setAutoScaleCanvas();
        Plot->setAxisScale(QwtPlot::xBottom, lowerBound, higherBound, 10);
    }
#endif
}




void GraphicWindow::on_ConnectedCheckBox_toggled(bool checked)
{
    if(checked)
        connect(pparent, SIGNAL(newMessage(_CANMsg *,int)), this, SLOT(newMessage(_CANMsg *,int)));
    else
        disconnect(pparent, SIGNAL(newMessage(_CANMsg *,int)), this, SLOT(newMessage(_CANMsg *,int)));

}

ofstream& GraphicWindow::operator>>(ofstream& os)
{
    //layout
    ExtRect e;
    QRect   q = this->geometry();
    e = &q;
    e >> os;

    //connection
    char connectedstate = (char)m_ui->ConnectedCheckBox->isChecked();
    os.put(connectedstate);

    //selected signals
    (*Sel) >> os;
    return os;
}
ifstream& GraphicWindow::operator<<(ifstream& is)
{
    //layout
    ExtRect e;
    e << is;
    this->setGeometry(e);

    //connection
    char connectedstate;
    is >> connectedstate;
    m_ui->ConnectedCheckBox->setChecked((bool)connectedstate);

    //selected signals

    (*Sel) << is;
    return is;
}

void GraphicWindow::on_AutoScalecheckBox_toggled(bool checked)
{
#if QWT_VERSION < 0x050200
    if(checked)
    {
        m_ui->FollowCheckBox->setEnabled(false);
        Plot->setAxisAutoScale(QwtPlot::xBottom);
        Plot->setAxisAutoScale(QwtPlot::yLeft);
    }
    else
    {
        m_ui->FollowCheckBox->setEnabled(true);

        int lowerBound = Plot->axisScaleDiv(QwtPlot::yLeft)->lBound();
        int higherBound = Plot->axisScaleDiv(QwtPlot::yLeft)->hBound();
        Plot->setAxisScale(QwtPlot::yRight, lowerBound, higherBound, 10);


        lowerBound = Plot->axisScaleDiv(QwtPlot::xBottom)->lBound();
        higherBound = Plot->axisScaleDiv(QwtPlot::xBottom)->hBound();
        Plot->setAxisScale(QwtPlot::xBottom, lowerBound, higherBound, 10);
    }
#else
    if(checked)
    {
        m_ui->FollowCheckBox->setEnabled(false);
        Plot->setAxisAutoScale(QwtPlot::xBottom);
        Plot->setAxisAutoScale(QwtPlot::yLeft);
    }
    else
    {
        m_ui->FollowCheckBox->setEnabled(true);

        int lowerBound = Plot->axisScaleDiv(QwtPlot::yLeft)->lowerBound();
        int higherBound = Plot->axisScaleDiv(QwtPlot::yLeft)->upperBound();
        Plot->setAxisScale(QwtPlot::yRight, lowerBound, higherBound, 10);


        lowerBound = Plot->axisScaleDiv(QwtPlot::xBottom)->lowerBound();
        higherBound = Plot->axisScaleDiv(QwtPlot::xBottom)->upperBound();
        Plot->setAxisScale(QwtPlot::xBottom, lowerBound, higherBound, 10);
    }
#endif
}

void GraphicWindow::on_YAutoScalecheckBox_toggled(bool checked)
{
#if QWT_VERSION < 0x050200
    if(checked)
    {
        Plot->setAxisAutoScale(QwtPlot::yLeft);
    }
    else
    {
        int lowerBound = Plot->axisScaleDiv(QwtPlot::yLeft)->lBound();
        int higherBound = Plot->axisScaleDiv(QwtPlot::yLeft)->hBound();
        Plot->setAxisScale(QwtPlot::yLeft, lowerBound, higherBound, 10);
    }
#else
    if(checked)
    {
        Plot->setAxisAutoScale(QwtPlot::yLeft);
    }
    else
    {
        int lowerBound = Plot->axisScaleDiv(QwtPlot::yLeft)->lowerBound();
        int higherBound = Plot->axisScaleDiv(QwtPlot::yLeft)->upperBound();
        Plot->setAxisScale(QwtPlot::yLeft, lowerBound, higherBound, 10);
    }
#endif
}
