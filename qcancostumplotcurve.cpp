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
#include "qcancostumplotcurve.h"
#include <QStyle>


QCANCostumPlotCurve::QCANCostumPlotCurve(QString TittleStr):QwtPlotCurve(TittleStr)
{
    d_data = new PrivateData();

}

QCANCostumPlotCurve::~QCANCostumPlotCurve()
{
    delete d_data;
}


void QCANCostumPlotCurve::setPen(const QPen &pen)
{
    if(pen != d_data->pen)
    {
        d_data->pen = pen;
        itemChanged();
    }
}

static int verifyRange(int size, int &i1, int &i2)
{
    if (size < 1)
        return 0;

    i1 = qwtLim(i1, 0, size-1);
    i2 = qwtLim(i2, 0, size-1);

    if ( i1 > i2 )
        qSwap(i1, i2);

    return (i2 - i1 + 1);
}
/*!
  \brief Draw an interval of the curve
  \param painter Painter
  \param xMap maps x-values into pixel coordinates.
  \param yMap maps y-values into pixel coordinates.
  \param from index of the first point to be painted
  \param to index of the last point to be painted. If to < 0 the
         curve will be painted to its last point.

  \sa drawCurve(), drawSymbols(),
*/
void QCANCostumPlotCurve::draw(QPainter *painter,
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    int from, int to) const
{
    if ( !painter || dataSize() <= 0 )
        return;

    if (to < 0)
        to = dataSize() - 1;

    if ( verifyRange(dataSize(), from, to) > 0 )
    {
        painter->save();
        painter->setPen(d_data->pen);

        /*
          Qt 4.0.0 is slow when drawing lines, but it's even
          slower when the painter has a brush. So we don't
          set the brush before we really need it.
         */

        drawCurve(painter, d_data->style, xMap, yMap, from, to);
        painter->restore();

        if (d_data->symbol->style() != QwtSymbol::NoSymbol)
        {
            painter->save();
            drawSymbols(painter, *d_data->symbol, xMap, yMap, from, to);
            painter->restore();
        }
    }
}

void QCANCostumPlotCurve::setStyle(CurveStyle style)
{
    if ( style != d_data->style )
    {
        d_data->style = style;
        itemChanged();
    }
}

void QCANCostumPlotCurve::setLinePen(const QPen &pen)
{
    d_data->Linepen = pen;
}

void QCANCostumPlotCurve::setDotPen(const QPen &pen)
{
    d_data->Dotpen = pen;
}

void QCANCostumPlotCurve::drawCurve(QPainter *painter, int style,
                                    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                                    int from, int to)const
{

    switch (style)
    {
    case Lines:
        if ( testCurveAttribute(Fitted) )
        {
            // we always need the complete
            // curve for fitting
            from = 0;
            to = dataSize() - 1;
        }
        painter->setPen(d_data->pen);
        drawLines(painter, xMap, yMap, from, to);
        break;
    case Sticks:
        drawSticks(painter, xMap, yMap, from, to);
        break;
    case Steps:
        drawSteps(painter, xMap, yMap, from, to);
        break;
    case Dots:
        drawDots(painter, xMap, yMap, from, to);
        break;
    case NoCurve:
        break;
    case 101:
        painter->setPen(d_data->Linepen);
        drawLines(painter, xMap, yMap, from, to);
        painter->setPen(d_data->Dotpen);
        drawDots(painter, xMap, yMap, from, to);
        painter->setPen(d_data->Linepen);
        break;
    default:
        break;
    }

}

const QPen& QCANCostumPlotCurve::pen() const
{
    return d_data->pen;
}

//!  Update the widget that represents the curve on the legend
void QCANCostumPlotCurve::updateLegend(QwtLegend *legend) const
{
    if ( !legend )
        return;

    QwtPlotItem::updateLegend(legend);

    QWidget *widget = legend->find(this);
    if ( !widget || !widget->inherits("QwtLegendItem") )
        return;

    QwtLegendItem *legendItem = (QwtLegendItem *)widget;

#if QT_VERSION < 0x040000
    const bool doUpdate = legendItem->isUpdatesEnabled();
#else
    const bool doUpdate = legendItem->updatesEnabled();
#endif
    legendItem->setUpdatesEnabled(false);

    const int policy = legend->displayPolicy();

    if (policy == QwtLegend::FixedIdentifier)
    {
        int mode = legend->identifierMode();

        if (mode & QwtLegendItem::ShowLine)
            legendItem->setCurvePen(pen());

        if (mode & QwtLegendItem::ShowSymbol)
            legendItem->setSymbol(symbol());

        if (mode & QwtLegendItem::ShowText)
            legendItem->setText(title());
        else
            legendItem->setText(QwtText());

        legendItem->setIdentifierMode(mode);
    }
    else if (policy == QwtLegend::AutoIdentifier)
    {
        int mode = 0;

        if (QwtPlotCurve::NoCurve != style())
        {
            QPen locPen(pen());
            locPen.setWidth(7);
            legendItem->setCurvePen(locPen);
            mode |= QwtLegendItem::ShowLine;
        }
        if (QwtSymbol::NoSymbol != symbol().style())
        {
            legendItem->setSymbol(symbol());
            mode |= QwtLegendItem::ShowSymbol;
        }
        if ( !title().isEmpty() )
        {
            legendItem->setText(title());
            mode |= QwtLegendItem::ShowText;
        }
        else
        {
            legendItem->setText(QwtText());
        }
        legendItem->setIdentifierMode(mode);
    }

    legendItem->setUpdatesEnabled(doUpdate);
    legendItem->update();
}
