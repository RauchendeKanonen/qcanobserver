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
