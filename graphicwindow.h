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
#ifndef GRAPHICWINDOW_H
#define GRAPHICWINDOW_H

#include <QtGui/QWidget>
#include "messagebufferinterface.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_zoomer.h>
#include <qwt.h>
#include <qwt_plot_panner.h>
#include <qwt_scale_div.h>
#include "processdatabase.h"
#include "signalselectordialog.h"
#include <Qt>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_symbol.h>
#include <qwt_text.h>
#include "cansignalcollection.h"
#include <iostream>
#include <fstream>

using namespace std;

#define MAX_GRAPH_WINDOWS 16

class QCanPlot : public QwtPlot
{
    public:
    QCanPlot(QString Name, QWidget *parent)
    {
        legend = new QwtLegend();
        legend->setItemMode(QwtLegend::ClickableItem);
        this->insertLegend(legend, QwtPlot::BottomLegend);
        ActCanvas = canvas();
        CanvasZoomer = new QwtPlotZoomer( QwtPlot::xBottom, QwtPlot::yLeft,  ActCanvas);
        CanvasZoomer->setSelectionFlags( QwtPicker::DragSelection );
        CanvasZoomer->setTrackerMode(QwtPicker::ActiveOnly);



        Panner = new QwtPlotPanner(canvas());
        Panner->setAxisEnabled(QwtPlot::yRight, false);
        Panner->setMouseButton(Qt::MidButton);

    }
    ~QCanPlot()
    {
        delete legend;
        delete CanvasZoomer;
        delete Panner;
    }
    //Left Mouse Button scales to the max autoscale
    void setAutoScaleCanvas()
    {
        this->setAxisAutoScale(QwtPlot::xBottom);
        this->setAxisAutoScale(QwtPlot::yLeft);
        ActCanvas = canvas();

        delete CanvasZoomer;
        CanvasZoomer = new QwtPlotZoomer( QwtPlot::xBottom, QwtPlot::yLeft,  ActCanvas);
        CanvasZoomer->setSelectionFlags( QwtPicker::DragSelection );
        CanvasZoomer->setTrackerMode(QwtPicker::ActiveOnly);
    }
    QwtPlotCanvas *ActCanvas;
    QwtPlotZoomer *CanvasZoomer;
    QwtPlotPanner *Panner;
    QwtLegend *legend;
};

class ItemCurveInfo
{
    public:
    ItemCurveInfo(CANSignal *Signal)
    {
	pSignal = Signal;
	PlotCurve = new QwtPlotCurve(pSignal->Name+QString(" ")+pSignal->Unit);
        PlotCurve->setItemAttribute(QwtPlotItem::Legend, true);
    }
    ~ItemCurveInfo()
    {
        delete PlotCurve;
    }

    QwtPlotCurve *PlotCurve;

    CANSignal *pSignal;
    QwtArray<double> x;
    QwtArray<double> y;
};

namespace Ui {
    class GraphicWindow;
}

class GraphicWindow : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(GraphicWindow)
public:
    explicit GraphicWindow(QWidget *parent = 0, CANSignalCollection *Collection = 0);
    virtual ~GraphicWindow();
    ofstream& operator>>(ofstream& os);
    ifstream& operator<<(ifstream& is);

protected:
    virtual void changeEvent(QEvent *e);
public slots:
    void MainTimerSlot(void);
    void addItemToDraw(CANSignal* Signal, QColor Color);
    void deleteItemToDraw(CANSignal* Signal);
    void newMessage(_CANMsg *, int);
    void ClearAll();
    void StopCapture();
private:
    Ui::GraphicWindow *m_ui;
    QList<ItemCurveInfo*> Curves;
    void closeEvent( QCloseEvent *e );
    CANSignalCollection *pCollection;
    SignalSelectorDialog *Sel;
    QCanPlot *Plot;
    bool Follow;
    float FollowTime;
    QWidget *pparent;

private slots:
    void on_YAutoScalecheckBox_toggled(bool checked);
    void on_AutoScalecheckBox_toggled(bool checked);
    void on_ConnectedCheckBox_toggled(bool checked);
    void on_FollowCheckBox_toggled(bool checked);
    void on_GraphFromDB_clicked();
    void on_GraphFromScratch_clicked();
};

#endif // GRAPHICWINDOW_H
