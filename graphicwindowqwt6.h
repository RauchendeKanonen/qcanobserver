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


#include <qwt_scale_widget.h>

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
#include <qwt_plot_magnifier.h>
#include <QLinkedList>
#include "configdialog.h"
#include <QMap>


using namespace std;

#define MAX_GRAPH_WINDOWS 16




class CurveData: public QwtArraySeriesData<QPointF>
{
public:
    CurveData()
    {
    }

    QRectF boundingRect() const
    {
        d_boundingRect = QRectF( 0.0, 0.0, -1.0, -1.0 );
        if(d_samples.size() < 1)
            return d_boundingRect;

        QRectF BoundingRect;
        BoundingRect.setBottom(d_samples.at(0).y());
        BoundingRect.setLeft(d_samples.at(0).x());
        BoundingRect.setTop(d_samples.at(0).y());
        BoundingRect.setRight(d_samples.at(d_samples.size()-1).x());

        for( int i = 0 ; d_samples.size() > i ; i ++ )
        {
            if(d_samples.at(i).y() > BoundingRect.top())
                BoundingRect.setTop(d_samples.at(i).y());

            if(d_samples.at(i).y() < BoundingRect.bottom())
                BoundingRect.setBottom(d_samples.at(i).y());

        }

        d_boundingRect = BoundingRect;
        return d_boundingRect;
    }

    inline void append( const QPointF &point )
    {
        d_samples += point;
    }

    void removeFirst(void)
    {
        d_samples.remove(0, 1);
    }

    void clear()
    {
        d_samples.clear();
        d_samples.squeeze();
        d_boundingRect = QRectF( 0.0, 0.0, -1.0, -1.0 );
    }
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
    void configChanged(__config cfg);
    void setConfig(int MemBytesMax);
    void saveSignalToFile(QString, CANSignal*);
    void MainTimerSlot(void);
    void addItemToDraw(CANSignal* Signal, QColor Color);
    void deleteItemToDraw(CANSignal* Signal);
    void newMessage(_CANMsg , int);
    void ClearAll();
    void StopCapture();
    void panned(int dx, int dy);
private:
    Ui::GraphicWindow *m_ui;
    void closeEvent( QCloseEvent *e );
    CANSignalCollection *pCollection;
    SignalSelectorDialog *Sel;
    QwtPlot *Plot;
    QMultiMap <QwtPlotCurve*, QString> CurveNameMapping;
    QMultiMap <int, QwtPlotCurve*> IdCurveMapping;
    QMultiMap <QwtPlotCurve*, CANSignal*> CurveSignalMapping;


    double ActTime;
    double StartTime;

    int MemBytesMax;
    QWidget *pparent;
    int MaxDots;
private slots:
    void on_pushButton_clicked();
    void on_checkBoxDottedLine_toggled(bool checked);
    void on_YAutoScalecheckBox_toggled(bool checked);
    void on_AutoScalecheckBox_toggled(bool checked);
    void on_ConnectedCheckBox_toggled(bool checked);
    void on_GraphFromDB_clicked();
    void on_GraphFromScratch_clicked();
};

#endif // GRAPHICWINDOW_H
