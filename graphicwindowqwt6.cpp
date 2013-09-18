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
#include "graphicwindowqwt6.h"
#include "ui_graphicwindowqwt6.h"
#include "extrect.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include <QBitmap>
#include <QPixmap>
#include <QIcon>
#include <qwt_plot_layout.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_engine.h>


int writeCSV(QString Path, QwtPlotCurve *Curve);

GraphicWindow::GraphicWindow(QWidget *parent, CANSignalCollection *Collection) :
    QWidget(NULL),
    m_ui(new Ui::GraphicWindow)
{
    pCollection = Collection;
    pparent = parent;
    m_ui->setupUi(this);
    Plot = new QwtPlot(QString(""), this);
    connect(Plot, SIGNAL(panned(int, int)), this, SLOT(panned(int, int)));
    m_ui->verticalLayout->addWidget(Plot);

    Plot->show();

    Plot->setAxisTitle(QwtPlot::xBottom,"Time s");


    Plot->setCanvasBackground(QColor(0x3d,0x93,0xfd,255));

    Sel = new SignalSelectorDialog(NULL, pCollection);
    connect(Sel, SIGNAL(addItemToDraw(CANSignal*, QColor)), this, SLOT(addItemToDraw(CANSignal*, QColor)));
    connect(Sel, SIGNAL(deleteItemToDraw(CANSignal*)), this, SLOT(deleteItemToDraw(CANSignal*)));
    connect(Sel, SIGNAL(saveSignalToFile(QString, CANSignal*)), this, SLOT(saveSignalToFile(QString, CANSignal*)));
    MaxDots = 0;
    MemBytesMax = 0;

    // legend
    QwtLegend *legend = new QwtLegend;
    legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
    Plot->insertLegend(legend, QwtPlot::BottomLegend);

    // axis
    Plot->setAxisTitle(QwtPlot::xBottom, "Time Axis");
    Plot->setAxisTitle(QwtPlot::yLeft, "Y Axis");
    ActTime = 1.0;
    StartTime = 0.0;
}

GraphicWindow::~GraphicWindow()
{

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
    for(i = 0 ; CurveNameMapping.count() >  i ; i++)
    {
        if(IdCurveMapping.contains(CANMsg.ID))
        {

            QList <QwtPlotCurve*> Curves = IdCurveMapping.values(CANMsg.ID);

            for( int m = 0 ; m < Curves.count() ; m ++ )
            {
                QwtPlotCurve *Curve = Curves.at(m);
                SignalDataCollection DataCol;
                CANSignal *Sig = CurveSignalMapping.value(Curve);
                Sig->getSignalDataCollection(CANMsg.DATA, &DataCol);
                QPointF Point((double)CANMsg.tv.tv_sec + (double)CANMsg.tv.tv_usec/1000000.0 ,DataCol.Value);

                CurveData *data = static_cast<CurveData *>( Curve->data() );
                data->append(Point);
                if(MaxDots && MaxDots < data->size())
                {
                    data->removeFirst();
                    if(data->sample(0).x() > StartTime)
                        StartTime = data->sample(0).x();
                }

                ActTime = (double)CANMsg.tv.tv_sec + (double)CANMsg.tv.tv_usec/1000000.0;
            }

        }
    }
}



void GraphicWindow::MainTimerSlot()
{
    QList <QwtPlotCurve*> Curvelist = CurveNameMapping.keys();
    QList <QRectF> RectList;
    QRectF United;

    if(Curvelist.count() == 0)
        return;

    for( int i = 0 ; i < Curvelist.count() ; i ++)
    {
        RectList.append(Curvelist.at(i)->boundingRect());
    }

    for( int i = 0 ; i < RectList.count() ; i ++)
    {
        if(RectList.at(i).width() > -1.0 && RectList.at(i).height() > -1.0)
        {
            United = RectList.at(i);
            break;
        }
    }

    for( int i = 0 ; i < RectList.count() ; i ++)
    {
        if(RectList.at(i).width() > -1.0 && RectList.at(i).height() > -1.0)
            United = United.unite(RectList.at(i));
    }

    if(United.height() < 0.00000001)
    {
        United.setTop(United.top()+0.25);
        United.setBottom(United.bottom()-0.25);
    }
    double upper, lower;
    if(United.bottom() < United.top())
    {
        lower = United.bottom();
        upper = United.top();
    }
    else
    {
        upper = United.bottom();
        lower = United.top();
    }

    Plot->setAxisScale(QwtPlot::yLeft, lower, upper);

    if(Plot->axisInterval(QwtPlot::xBottom).maxValue() <= United.right() || Plot->axisInterval(QwtPlot::xBottom).minValue() <= United.left())
        Plot->setAxisScale(QwtPlot::xBottom, United.left(), United.right()+(United.right()-United.left())/5);

    Plot->replot();
}

void GraphicWindow::ClearAll()
{
    disconnect(pparent, SIGNAL(newMessage(_CANMsg ,int)), this, SLOT(newMessage(_CANMsg ,int)));

    QList <QwtPlotCurve*> CurveList = CurveNameMapping.keys();

    for(int i = 0 ; i < CurveList.count() ; i ++ )
    {
        QwtPlotCurve *Curve = CurveList.at(i);
        CurveData *data = static_cast<CurveData *>( Curve->data() );
        data->clear();
    }
    connect(pparent, SIGNAL(newMessage(_CANMsg ,int)), this, SLOT(newMessage(_CANMsg ,int)));
    Plot->replot();
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
    disconnect(pparent, SIGNAL(newMessage(_CANMsg ,int)), this, SLOT(newMessage(_CANMsg ,int)));
    QBrush br(Qt::NoBrush);
    br.setColor(Color);
    QPen Pen(Color);
    QwtPlotCurve *NewCurve = new QwtPlotCurve(Signal->Name);
    NewCurve->setBrush(br);
    Pen.setWidth(1);
    NewCurve->setPen(Pen);
    Pen.setWidth(1);

    CurveNameMapping.insert(NewCurve, Signal->Name);
    IdCurveMapping.insert(Signal->Id, NewCurve);
    CurveSignalMapping.insert(NewCurve, Signal);
    NewCurve->setData(new CurveData());
    NewCurve->setPen(Pen);
    NewCurve->attach(Plot);
    setConfig(MemBytesMax);


    connect(pparent, SIGNAL(newMessage(_CANMsg ,int)), this, SLOT(newMessage(_CANMsg ,int)));
}

void GraphicWindow::deleteItemToDraw(CANSignal* Signal)
{
    disconnect(pparent, SIGNAL(newMessage(_CANMsg ,int)), this, SLOT(newMessage(_CANMsg ,int)));

    QwtPlotCurve * Curve = CurveSignalMapping.key(Signal);
    if(Curve)
        Curve->detach();

    delete Curve;


    CurveNameMapping.remove(Curve);
    CurveSignalMapping.remove(Curve);
    IdCurveMapping.remove(Signal->Id, Curve);



    setConfig(MemBytesMax);

    connect(pparent, SIGNAL(newMessage(_CANMsg ,int)), this, SLOT(newMessage(_CANMsg ,int)));
}

void GraphicWindow::StopCapture()
{
    Plot->replot();
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

}

void GraphicWindow::on_YAutoScalecheckBox_toggled(bool checked)
{
#if QWT_VERSION < 0x050200

#else

#endif
}

void GraphicWindow::panned(int dx, int dy)
{

}

void GraphicWindow::on_checkBoxDottedLine_toggled(bool checked)
{
    int i;
    QList <QwtPlotCurve*> Curves = CurveNameMapping.keys();

    if(checked)
    {
        QBrush Brush(3);
        QPen   Pen(3);
        for( i = 0 ; Curves.count() > i ; i++ )
        {
            Pen.setColor(Curves.at(i)->pen().color());
            Brush.setColor(Curves.at(i)->pen().color());
            Curves.at(i)->setSymbol(new QwtSymbol(QwtSymbol::Rect,Brush, Pen,QSize(3,3)));
        }
        Plot->replot();
    }
    else
    {
        for( i = 0 ; Curves.count() > i ; i++ )
            Curves.at(i)->setSymbol(NULL);
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

    writeCSV(Path, CurveSignalMapping.key(Sig));
}

int writeCSV(QString Path, QwtPlotCurve *Curve)
{
    QwtSeriesData <QPointF> *Data = Curve->data();

    int LengthOfBuf = Curve->data()->size() * (2*32+4); // 32Bytes per float x,y and " : " "\n

    char *FileBuf = (char*)malloc(LengthOfBuf);
    char *ptr = FileBuf;

    memset(ptr, 0, LengthOfBuf);

    int OUTFILE;

    for(int i = 0; i < Curve->data()->size() ; i++)
    {
        ptr += sprintf(ptr,"%f : %f\n", Data->sample(i).x(), Data->sample(i).y());
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
        MaxDots = MemBytesMax/(CurveNameMapping.count()*2*sizeof(double));
    else
        MaxDots = 0;
}
//recalc
void GraphicWindow::setConfig(int AMemBytesMax)
{
    int Count = CurveNameMapping.count();
    if(Count == 0)
        Count = 1;
    MemBytesMax = AMemBytesMax;
    if( 0 != MemBytesMax)
        MaxDots = MemBytesMax/(Count*2*sizeof(double));
    else
        MaxDots = 0;
}
