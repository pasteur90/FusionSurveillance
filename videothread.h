#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QThread>
#include <opencv2/opencv.hpp>

class VideoThread : public QThread
{
    Q_OBJECT
public:
    explicit VideoThread(QObject *parent = 0);

signals:
    void imageReady(const cv::Mat &);

public slots:
    void run();

private:
    cv::VideoCapture m_vcap;
    cv::Mat m_image;
    bool m_quit;

};

#endif // VIDEOTHREAD_H
