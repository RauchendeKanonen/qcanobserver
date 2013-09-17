/*


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
#ifndef QCANCOSTUMPLOTCURVE_H
#define QCANCOSTUMPLOTCURVE_H
#include <QtGui/QWidget>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_zoomer.h>
#include <qwt.h>
#include <qwt_plot_panner.h>
#include <qwt_scale_div.h>
#include <Qt>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_symbol.h>
#include <qwt_text.h>
#include <QBitArray>
#include <QBitArray>
#include <QPointer>
#include <QPainter>
#include <QMap>
#include <QEvent>
#include <qwt_spline.h>
#include <qwt_curve_fitter.h>
#include <qwt_painter.h>


class QCANCostumPlotCurve : public QwtPlotCurve
{
class PrivateData
{
public:
    class PixelMatrix: private QBitArray
    {
    public:
        PixelMatrix(const QRect& rect):
                QBitArray(rect.width() * rect.height()),
                _rect(rect)
        {
            fill(false);
        }

        inline bool testPixel(const QPoint& pos)
        {
            if ( !_rect.contains(pos) )
                return false;

            const int idx = _rect.width() * (pos.y() - _rect.y()) +
                            (pos.x() - _rect.x());

            const bool marked = testBit(idx);
            if ( !marked )
                setBit(idx, true);

            return !marked;
        }

    private:
        QRect _rect;
    };

    PrivateData():
            curveType(Yfx),
            style(QwtPlotCurve::Lines),
            reference(0.0),
            attributes(0),
            paintAttributes(0)
    {
        symbol = new QwtSymbol();
        pen = QPen(Qt::black);
        curveFitter = new QwtSplineCurveFitter;
    }

    ~PrivateData()
    {
        delete symbol;
        delete curveFitter;
    }

    QwtPlotCurve::CurveType curveType;
    QwtPlotCurve::CurveStyle style;
    double reference;

    QwtSymbol *symbol;
    QwtCurveFitter *curveFitter;

    QPen pen;
    QPen Dotpen;
    QPen Linepen;
    QBrush brush;

    int attributes;
    int paintAttributes;

    //QwtGuardedPainter guardedPainter;
};

private:
    void draw(QPainter *painter,
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    int from, int to) const;

    
    PrivateData *d_data;
public:
    const QPen& pen() const ;
    void updateLegend(QwtLegend *legend) const;
    void setStyle(CurveStyle style);
    QCANCostumPlotCurve(QString TittleStr);
    ~QCANCostumPlotCurve();
    void setPen(const QPen &pen);
    void setDotPen(const QPen &pen);
    void setLinePen(const QPen &pen);
    void drawCurve(QPainter *painter, int style,
                   const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                   int from, int to)const;
};

#endif // QCANCOSTUMPLOTCURVE_H
