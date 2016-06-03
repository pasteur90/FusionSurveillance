#ifndef RADARPLOT_H
#define RADARPLOT_H

#include <QWidget>
#include <QVector>
#include <QMap>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include "srr208track.h"
#include "curvedata.h"

#define NUM_TRACK 25

class RadarPlot : public QwtPlot
{
    Q_OBJECT
public:
    RadarPlot(QWidget *parent = NULL);
    ~RadarPlot();
    enum SRR208Option {
        NoOptions = 0x00,
        OpVRelLongEn = 0x01,
        OpVRelLatEn = 0x02,
        OpLongDisplDiffEn = 0x04,
        OpLatDisplDiffEn = 0x08,
        OpLifeTimeEn = 0x10
    };
    Q_DECLARE_FLAGS(SRR208Options, SRR208Option)

private:
    QwtPlotCurve *m_curve;
    CurveData *m_data;
    QwtPlotCurve *m_grayCurve;
    CurveData *m_grayData;
    QwtPlotMarker *m_label[NUM_TRACK];
    bool filter(const SRR208TrackT &track);
    void getOptions();
    SRR208Options m_options;
    double m_rcsThresh;
    double m_normToLifeTimeThresh;
    double m_longDisplDiffThresh;
    double m_latDisplDiffThresh;
    double m_lifeTimeThresh;


public slots:
    void appendData(double x, double y);
    void appendGrayData(double x, double y);
    void removeData();
    //void onTrackVectorReady(QVector<SRR208TrackT> *trackVector);
    void onTrackMapReady(QMap<int, SRR208TrackT> *trackMap);

};

Q_DECLARE_OPERATORS_FOR_FLAGS(RadarPlot::SRR208Options)

#endif // RADARPLOT_H
