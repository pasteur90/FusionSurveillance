#ifndef LIDARPLOT_H
#define LIDARPLOT_H

#include <QWidget>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_grid.h>
#include "curvedata.h"

#define NUM_TRACK 25
class LidarPlot : public QwtPlot
{
    Q_OBJECT
public:
    explicit LidarPlot(QWidget *parent = 0);
    ~LidarPlot();
private:
    QwtPlotCurve *m_curve;
    CurveData *m_data;
    QwtPlotMarker *m_label[NUM_TRACK];
//    QwtPlotGrid *grid;

signals:

public slots:
    void appendData(double x, double y);
};

#endif // LIDARPLOT_H
