#include "lidarplot.h"
#include <qwt_plot_curve.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_symbol.h>

LidarPlot::LidarPlot(QWidget *parent) :
    QwtPlot(parent),
    m_curve(NULL),
    m_data(NULL)
{
    setAutoReplot(false);
    setMaximumWidth(400);
    setMaximumHeight(400);
//    setCanvasBackground(QColor(29, 100, 141));
    setCanvasBackground(Qt::black);
    setAxisScale(QwtPlot::yLeft, -0.3, 3.5);
    setAxisScale(QwtPlot::xBottom, -0.3, 1.3);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen(QPen(Qt::gray));
    grid->attach(this);

    for (int i = 0; i < NUM_TRACK; i++) {
        m_label[i] = new QwtPlotMarker();
        m_label[i]->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
        m_label[i]->attach(this);
    }

    appendData(-5, 15);
    QwtText text;
    text.setText(QString("자동차"));
//    text.setColor(Qt::white);
//    m_label[0]->setLabel(text);
//    m_label[0]->setXValue(-5);
//    m_label[0]->setYValue(15);
//    m_label[0]->show();

//    float car_height[51] = {0.32467532,  0.46753247,  0.56493506,  0.57792208,  0.5974026 ,
//                            0.61688312,  0.62337662,  0.64285714,  0.65584416,  0.68181818,
//                            0.68181818,  0.7012987 ,  0.7012987 ,  0.71428571,  0.75324675,
//                            0.77922078,  0.81168831,  0.85714286,  0.87662338,  0.9025974 ,
//                            0.92857143,  0.95454545,  0.96753247,  0.98051948,  0.98701299,
//                            1.        ,  0.99350649,  1.        ,  1.        ,  1.        ,
//                            1.        ,  0.99350649,  0.99350649,  0.99350649,  0.98051948,
//                            1.        ,  0.95454545,  0.92857143,  0.90909091,  0.8961039 ,
//                            0.87662338,  0.90909091,  0.82467532,  0.80519481,  0.77272727,
//                            0.75324675,  0.74675325,  0.75324675,  0.74675325,  0.45454545,  0. };

    float car_height[51] = {0.33155325,  0.51642332,  0.56055749,  0.58293582,  0.59318516,
                            0.60822371,  0.61525737,  0.62753517,  0.67814184,  0.67237415,
                            0.68769552,  0.69859486,  0.69737153,  0.70647506,  0.75245302,
                            0.7627759 ,  0.80644112,  0.85338409,  0.90480405,  0.90469332,
                            0.92470253,  0.94353727,  1.01323338,  0.98780667,  1.02945218,
                            1.02198858,  0.98912754,  1.0017217 ,  1.016509  ,  0.9884539 ,
                            1.00926592,  0.99290723,  0.99040841,  1.00000167,  0.99262796,
                            0.97079362,  0.93691823,  0.93956549,  0.88230477,  0.85595725,
                            0.90736583,  0.93351886,  0.8084159 ,  0.8160701 ,  0.75585208,
                            0.78540315,  0.71967628,  0.75759306,  0.75877411,  0.48299567,
                            0.00368966};


    for (int i = 0; i < 51; i++) {
        appendData(i * 0.02, car_height[i]);
    }

}

void LidarPlot::appendData(double x, double y)
{
    if (m_data == NULL)
        m_data = new CurveData();

    if (m_curve == NULL) {
        m_curve = new QwtPlotCurve("test");
        m_curve->setStyle(QwtPlotCurve::NoCurve);

        const QColor &c = Qt::red;
        m_curve->setSymbol(new QwtSymbol(QwtSymbol::Rect, QBrush(c), QPen(c), QSize(5, 5)));

        m_curve->attach(this);
    }

    m_data->append(x, y);
    m_curve->setRawSamples(m_data->x(), m_data->y(), m_data->count());
}

LidarPlot::~LidarPlot()
{

}

