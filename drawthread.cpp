#include "drawthread.h"
#include "candevice.h"
#include "messagebufferinterface.h"
#include "FilterDialog.h"
#include "canframedataselektor.h"

DrawThread::DrawThread()
{
    Graph = new GraphicWindow(NULL);
    Graph->show();
}
DrawThread::~DrawThread()
{
    delete Graph;
}


void DrawThread::run()
{
    TPCANMsg Msg;
    QuitNow = 0;


    while(1)
    {
        if(QuitNow)
        {
            return;
        }
    }
}

void DrawThread::QuitThread()
{
    QuitNow = 1;
}

void DrawThread::AddGraphFromScratch(int AId, int AMask[8])
{
    GraphSet->append(new DataGraph(AId, AMask));
}

 void DrawThread::setFilter(int Place, int from, int to)
 {
    if(Place == GRAPHICWINFILTER)
    {

    }

    //clear all
    if(Place == GRAPHICWINFILTER && from == -1 && to == -1)
    {

    }
 }
