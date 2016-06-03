#include "radarplot.h"
#include <qwt_plot_curve.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_shapeitem.h>
#include <QSettings>

RadarPlot::RadarPlot(QWidget *parent):
    QwtPlot(parent),
    m_curve(NULL),
    m_data(NULL),
    m_grayCurve(NULL),
    m_grayData(NULL)
{
    setAutoReplot(false);
//  setCanvasBackground(QColor(29, 100, 141));
    setCanvasBackground(Qt::black);
    setAxisScale(QwtPlot::yLeft, -5.0, 50.0);
    setAxisScale(QwtPlot::xBottom, -50.0, 50.0);
    setMaximumWidth(400);
    setMaximumHeight(400);
    QwtPlotShapeItem *sItem = new QwtPlotShapeItem();
    sItem->setRect(QRectF(-6.25, 16, 2.5, 4));
    sItem->setPen(QPen(QBrush(Qt::red), 3));
    sItem->attach(this);
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen(QPen(Qt::gray));
    grid->attach(this);

    appendData(5, 50);
    appendData(5, 47);
    appendData(5.1, 44);
    appendData(5.2, 41);
    appendData(5.2, 38);
    appendData(5.1, 35);
    appendData(5.1, 32);
    appendData(5.1, 29);
    appendData(5, 27);
    appendData(5, 25);
    appendData(5, 23);
    appendData(5, 21);

    QwtPlotMarker *marker = new QwtPlotMarker();
    marker->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
    marker->attach(this);
    QwtText text("[4938] Car 3m/s\n (-5.675, 15.000)");
    text.setFont(QFont("Monospace", 12, QFont::Bold));
    text.setColor(Qt::red);
    marker->setXValue(-4.5);
    marker->setYValue(15);
    marker->setLabel(text);
    marker->show();

    for (int i = 0; i < NUM_TRACK; i++)
    {
        m_label[i] = new QwtPlotMarker();
        m_label[i]->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
        m_label[i]->attach(this);
    }
}


bool RadarPlot::filter(const SRR208TrackT &track)
{
    double norm = qAbs(track.longDisplAcc) + qAbs(track.latDisplAcc);

    double latDiff = track.latDispl - track.latDisplLast;
    double longDiff = track.longDispl - track.longDisplLast;
    double diffSquared = latDiff * latDiff + longDiff * longDiff;

    if (track.vRelLat == 0 && diffSquared > m_latDisplDiffThresh)
        return false;
    if (track.vRelLong == 0 && diffSquared > m_longDisplDiffThresh)
        return false;

    if (track.lifeTime < m_lifeTimeThresh)
        return false;

    double var;
    if (track.n == 1)
        var = 0;
    else {
        double mu = track.RCSSum / track.n;
        var = (track.RCSSumOfSq - 2.0 * mu * track.RCSSum + mu * mu * track.n) / track.n;
    }
    if (m_rcsThresh > var) {
        return false;
    }
    if (m_normToLifeTimeThresh > norm / track.lifeTime) {
        return false;
    }
    return true;
}


void RadarPlot::appendData(double x, double y)
{
    if (m_data == NULL)
    {
        m_data = new CurveData();
    }

    if (m_curve == NULL)
    {
        m_curve = new QwtPlotCurve("test");
        m_curve->setStyle(QwtPlotCurve::NoCurve);

        const QColor &c = Qt::red;
        m_curve->setSymbol(new QwtSymbol(QwtSymbol::Rect,
                                     QBrush(c), QPen(c), QSize(5, 5)));

        m_curve->attach(this);
    }

    m_data->append(-x, y);
    m_curve->setRawSamples(m_data->x(), m_data->y(), m_data->count());

}

void RadarPlot::appendGrayData(double x, double y)
{
    if (m_grayData == NULL)
        m_grayData = new CurveData();

    if (m_grayCurve == NULL) {
        m_grayCurve = new QwtPlotCurve("gray");
        m_grayCurve->setStyle(QwtPlotCurve::NoCurve);

        const QColor &c = Qt::gray;
        m_grayCurve->setSymbol(new QwtSymbol(QwtSymbol::Rect, QBrush(c), QPen(c), QSize(5, 5)));

        m_grayCurve->attach(this);
    }

    m_grayData->append(-x, y);
    m_grayCurve->setRawSamples(m_grayData->x(), m_grayData->y(), m_grayData->count());
}


void RadarPlot::onTrackMapReady(QMap<int, SRR208TrackT> *trackMap)
{
    removeData();

    int size = trackMap->size();
    qDebug() << "size " << size;
    Q_ASSERT(size <= 25);
    int _i = 0;

    //parse options
    getOptions();

    QMap<int, SRR208TrackT>::const_iterator i;
    for (i = trackMap->constBegin(); i != trackMap->constEnd(); ++i) {
        const SRR208TrackT &track = i.value();

        const int id = i.key();
        double longDispl = static_cast<double>(track.longDispl);
        double latDispl = static_cast<double>(track.latDispl);

        float lifeTime = track.lifeTime;
        float velLong = track.vRelLong;
        float velLat = track.vRelLat;
        double var;
        if (track.n == 1)
            var = 0;
        else {
            double mu = track.RCSSum / track.n;
            var = (track.RCSSumOfSq - 2.0 * mu * track.RCSSum + mu * mu * track.n) / track.n;
        }
        double norm = qAbs(track.longDisplAcc) + qAbs(track.latDisplAcc);

        QwtText text;
        /*text.setText(QString("[") + QString::number(id) + QString("] : ") + QString::number(var, 'g', 2) + QString(", ")
                     + QString::number(norm, 'g', 2)
                     + QString("\n, ") + QString::number(lifeTime, 'g', 2) + QString(", ")
                     + QString::number(velLong, 'g', 2) + QString(",") + QString::number(velLat, 'g', 2));*/
//        text.setText(QString("[") + QString::number(id) + QString("] : ") + QString::number(norm/lifeTime, 'g', 2)
//                     + QString(", ") + QString::number(track.RCSSum / track.n, 'g', 2));
//        text.setText(QString("[") + QString::number(id) + QString("] : ") + QString::number(norm/lifeTime, 'g', 2));
        text.setText(QString("[") + QString::number(id) + QString("]"));

        //filter
        if (filter(track)) {
            appendData(latDispl, longDispl);
            text.setColor(Qt::white);
        }
        else {
            appendGrayData(latDispl, longDispl);
            text.setColor(Qt::gray);
        }
        m_label[_i]->setLabel(text);
        m_label[_i]->setXValue(-latDispl);
        m_label[_i]->setYValue(longDispl);
        m_label[_i]->show();
        _i++;
    }

    for (; _i< NUM_TRACK; _i++)
    {
        m_label[_i]->hide();
    }

    replot();

}

void RadarPlot::removeData()
{
    if (m_curve)
    {
        m_curve->detach();
    }

    if (m_grayCurve)
    {
        m_grayCurve->detach();
    }

    delete m_curve;
    m_curve = NULL;

    delete m_grayCurve;
    m_grayCurve = NULL;

    delete m_data;
    m_data = NULL;

    delete m_grayData;
    m_grayData = NULL;

   // replot();
}

void RadarPlot::getOptions()
{
    QSettings settings;
//    m_options = RadarPlot::NoOptions;
//    if (settings.value("filter/vRelLongEnable").toBool())
//        m_options |= RadarPlot::OpVRelLongEn;
//    if (settings.value("filter/vRelLatEnable").toBool())
//        m_options |= RadarPlot::OpVRelLatEn;
//    if (settings.value("filter/longDisplDiffEnable").toBool())
//        m_options |= RadarPlot::OpLongDisplDiffEn;
//    if (settings.value("filter/latDisplDiffEnable").toBool())
//        m_options |= RadarPlot::OpLatDisplDiffEn;
//    if (settings.value("filter/lifeTimeEnable").toBool())
//        m_options |= RadarPlot::OpLifeTimeEn;

    m_longDisplDiffThresh = settings.value("filter/longDisplDiff").toDouble();
    m_latDisplDiffThresh = settings.value("filter/latDisplDiff").toDouble();
    m_lifeTimeThresh = settings.value("filter/lifeTime").toDouble();
    m_rcsThresh = settings.value("filter/RCS").toDouble();
    m_normToLifeTimeThresh = settings.value("filter/normToLifeTime").toDouble();

}

RadarPlot::~RadarPlot()
{

}
