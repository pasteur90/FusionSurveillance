#ifndef CURVEDATA_H
#define CURVEDATA_H
#include <qwt_plot.h>

class CurveData
{
    // A container class for growing data
public:

    CurveData();

    void append(double *x, double *y, int count);
    void append(double x, double y);

    int count() const;
    int size() const;
    const double *x() const;
    const double *y() const;

private:
    int d_count;
    QVector<double> d_x;
    QVector<double> d_y;
};



#endif // CURVEDATA_H

