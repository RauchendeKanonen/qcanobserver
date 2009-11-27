#include<QThread>
#include<QObject>
#ifndef DRAWTHREAD_H
#define DRAWTHREAD_H
#include "graphicwindow.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

class DataGraph : public QwtPlotCurve
{
    public:
    DataGraph(int AId, int AMask[8])
    {
        id = AId;
        memcpy(Mask, AMask, 8*sizeof(AMask));
    }
    int id;
    int Mask[8];
};


class DrawThread : public QThread
{
     Q_OBJECT

private:
    int QuitNow;
    GraphicWindow *Graph;
    QList <DataGraph*> *GraphSet;
public:
    DrawThread();
    ~DrawThread();
    void setFilter(int Place, int from, int to);
    void run();
public slots:
    void QuitThread();
    void AddGraphFromScratch(int, int*);
};

#endif // DRAWTHREAD_H
