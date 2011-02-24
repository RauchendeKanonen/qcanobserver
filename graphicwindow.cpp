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
#include <stdio.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include <QBitmap>
#include <QPixmap>
#include <QIcon>


int writeCSV(QString Path, ItemCurveInfo *Curve);

GraphicWindow::GraphicWindow(QWidget *parent, CANSignalCollection *Collection) :
    QWidget(NULL),
    m_ui(new Ui::GraphicWindow)
{
    pCollection = Collection;
    pparent = parent;
    m_ui->setupUi(this);
    Plot = new QCanPlot(QString(""), this);
    connect(Plot, SIGNAL(panned(int, int)), this, SLOT(panned(int, int)));
    m_ui->verticalLayout->addWidget(Plot);

    Plot->show();

    Plot->setAxisTitle(QwtPlot::xBottom,"Time s");
    Plot->setAxisAutoScale(QwtPlot::xBottom);

    Sel = new SignalSelectorDialog(NULL, pCollection);
    connect(Sel, SIGNAL(addItemToDraw(CANSignal*, QColor)), this, SLOT(addItemToDraw(CANSignal*, QColor)));
    connect(Sel, SIGNAL(deleteItemToDraw(CANSignal*)), this, SLOT(deleteItemToDraw(CANSignal*)));
    connect(Sel, SIGNAL(saveSignalToFile(QString, CANSignal*)), this, SLOT(saveSignalToFile(QString, CANSignal*)));
    MaxDots = 0;
    MemBytesMax = 0;

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

void GraphicWindow::newMessage(_CANMsg CANMsg, int Cnt)
{
    int i;
    for(i = 0 ; Curves.count() >  i ; i++)
    {
        if(CANMsg.ID == (DWORD)Curves.at(i)->pSignal->Id)
        {
            SignalDataCollection DataCol;

            if(Curves.at(i)->pSignal->getSignalDataCollection(CANMsg.DATA, &DataCol))
            {
                Curves.at(i)->y.append(DataCol.Value);
                Curves.at(i)->x.append((double)CANMsg.tv.tv_sec + (double)CANMsg.tv.tv_usec/1000000.0);

                if(MaxDots != 0 && Curves.at(i)->y.count() > MaxDots)
                {
                    int rem = Curves.at(i)->y.count() - MaxDots;
                    Curves.at(i)->y.remove(0,rem);
                    Curves.at(i)->x.remove(0,rem);
                }
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
    disconnect(pparent, SIGNAL(newMessage(_CANMsg ,int)), this, SLOT(newMessage(_CANMsg ,int)));

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

    connect(pparent, SIGNAL(newMessage(_CANMsg ,int)), this, SLOT(newMessage(_CANMsg ,int)));
}


void GraphicWindow::on_GraphFromScratch_clicked()
{

}

void GraphicWindow::on_GraphFromDB_clicked()
{
    Sel->switchMode(SELECTORMODE);
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
    Pen.setWidth(1);
    Curves.last()->PlotCurve->setPen(Pen);
    Pen.setWidth(3);
    Curves.last()->PlotCurve->setDotPen(Pen);
    Pen.setWidth(1);
    Curves.last()->PlotCurve->setLinePen(Pen);
    Curves.last()->PlotCurve->updateLegend(Plot->legend);
    setConfig(MemBytesMax);
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
    setConfig(MemBytesMax);
}
void GraphicWindow::StopCapture()
{
    Plot->setAutoScaleCanvas();
}


void GraphicWindow::on_ConnectedCheckBox_toggled(bool checked)
{
    if(checked)
        connect(pparent, SIGNAL(newMessage(_CANMsg ,int)), this, SLOT(newMessage(_CANMsg ,int)));
    else
        disconnect(pparent, SIGNAL(newMessage(_CANMsg ,int)), this, SLOT(newMessage(_CANMsg ,int)));

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
        Plot->setAxisAutoScale(QwtPlot::xBottom);
        Plot->setAxisAutoScale(QwtPlot::yLeft);
    }
    else
    {
        Plot->setAutoScaleCanvas();
    }
#else
    if(checked)
    {
        Plot->setAxisAutoScale(QwtPlot::xBottom);
        Plot->setAxisAutoScale(QwtPlot::yLeft);
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

void GraphicWindow::panned(int dx, int dy)
{

}

void GraphicWindow::on_checkBoxDottedLine_toggled(bool checked)
{
    int i;
    if(checked)
    {
        for( i = 0 ; Curves.count() > i ; i++ )
            Curves.at(i)->PlotCurve->setStyle(QwtPlotCurve::CurveStyle(101));
        Plot->replot();
    }
    else
    {
        for( i = 0 ; Curves.count() > i ; i++ )
            Curves.at(i)->PlotCurve->setStyle(QwtPlotCurve::CurveStyle(1));
        Plot->replot();
    }
}

void GraphicWindow::on_pushButton_clicked()
{
    Sel->switchMode(SINGLEITEM);
    Sel->show();
}

void GraphicWindow::saveSignalToFile(QString Path, CANSignal* Sig)
{
    if(Sig == NULL)
    {
        Sel->hide();
        Sel->switchMode(SELECTORMODE);      //Switch back
        return;
    }

    int i;
    for(i = 0 ; Curves.count() >  i ; i++)
    {
        if(Sig->Id == Curves.at(i)->pSignal->Id
           && Sig->Name == Curves.at(i)->pSignal->Name)
        {
            writeCSV(Path, Curves.at(i));
            Sel->hide();
            Sel->switchMode(SELECTORMODE);      //Switch back
            return;
            break;
        }
    }
}

int writeCSV(QString Path, ItemCurveInfo *Curve)
{
    int LengthOfBuf = Curve->x.count() * (2*32+4); // 32Bytes per float x,y and " : " "\n

    char *FileBuf = (char*)malloc(LengthOfBuf);
    char *ptr = FileBuf;

    memset(ptr, 0, LengthOfBuf);

    int OUTFILE;

    for(int i = 0; i < Curve->x.count() ; i++)
    {
        ptr += sprintf(ptr,"%f : %f\n", Curve->x.at(i), Curve->y.at(i));
    }
#ifdef LINUX
    if((OUTFILE = open(Path.toStdString().c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU|S_IRWXG|S_IRWXO))<=0)
      return -1;
#endif

#ifdef WINDOWS
    if((OUTFILE = open(Path.toStdString().c_str(),O_CREAT | O_WRONLY |O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE))<=0)
        return -1;
#endif


    int flength = write(OUTFILE, FileBuf, ptr - FileBuf);
    close(OUTFILE);

    free(FileBuf);
    return flength;
}

void GraphicWindow::configChanged(__config cfg)
{
        MemBytesMax = cfg.GraphMemByte;
    if( 0 != MemBytesMax)
        MaxDots = MemBytesMax/(Curves.count()*2*sizeof(double));
    else
        MaxDots = 0;
}
//recalc
void GraphicWindow::setConfig(int AMemBytesMax)
{
        MemBytesMax = AMemBytesMax;
    if( 0 != MemBytesMax)
        MaxDots = MemBytesMax/(Curves.count()*2*sizeof(double));
    else
        MaxDots = 0;
}
