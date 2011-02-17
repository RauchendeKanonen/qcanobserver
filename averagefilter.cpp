#include "averagefilter.h"

AverageFilter::AverageFilter(int length)
{
    NumOfPoints = length;
}


void AverageFilter::addPoint(float Point)
{
    Signal.append(Point);
    if(Signal.count() > NumOfPoints)
        Signal.removeFirst();
}


float AverageFilter::getPoint(void)
{
    float val = 0;
    int i;
    for(i = 0 ; i < Signal.count() ; i ++ )
    {
        val += Signal.at(i);

    }
    val /= NumOfPoints;

    return val;
}
