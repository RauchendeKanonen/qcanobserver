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
#include "processdatabase.h"
#include "candataitemselector.h"
#include <Qt>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_symbol.h>
#include <qwt_text.h>

#define MAX_GRAPH_WINDOWS 16

class QCanPlot : public QwtPlot
{
    public:
    QCanPlot(QString Name, QWidget *parent) : QwtPlot(Name , parent)
    {
        QwtLegend *legend = new QwtLegend();
        legend->setItemMode(QwtLegend::ClickableItem);
        this->insertLegend(legend, QwtPlot::BottomLegend);
    }
};

class ItemCurveInfo
{
    public:
    ItemCurveInfo(CalRule *Set, int ARule)
    {
        Rule = ARule;
        Ruleset = Set;
        PlotCurve = new QwtPlotCurve(Ruleset->getName(Rule)+QString(" ")+Ruleset->getUnit(Rule));
        PlotCurve->setItemAttribute(QwtPlotItem::Legend, true);
    }
    ~ItemCurveInfo()
    {
        delete PlotCurve;
    }

    QwtPlotCurve *PlotCurve;
    int Rule;
    CalRule *Ruleset;
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
    explicit GraphicWindow(QWidget *parent = 0, QList<CalRule*> *RuleList = 0);
    virtual ~GraphicWindow();
protected:
    virtual void changeEvent(QEvent *e);
public slots:
    void MainTimerSlot(void);
    void addItemToDraw(CalRule*, int, QColor);
    void newMessage(CANMsgandTimeStruct *, int);
    void ClearAll();
private:
    Ui::GraphicWindow *m_ui;
    QList<ItemCurveInfo*> Curves;
    QList<CalRule*> *pRuleList;
    CANDataItemSelector *Sel;
    QCanPlot *Plot;
private slots:
    void on_GraphFromDB_clicked();
    void on_GraphFromScratch_clicked();
};

#endif // GRAPHICWINDOW_H
