#ifndef AVERAGEFILTER_H
#define AVERAGEFILTER_H

#include <QList>

class AverageFilter
{
public:
    int NumOfPoints;
    QList <float> Signal;
    AverageFilter(int length);
    void addPoint(float Point);
    float getPoint(void);
};

#endif // AVERAGEFILTER_H
