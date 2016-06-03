#include "videothread.h"
#include <QDebug>

VideoThread::VideoThread(QObject *parent) : QThread(parent)
  ,m_quit(false)
{
    if (!m_vcap.open("rtsp://184.72.239.149/vod/mp4:BigBuckBunny_115k.mov")) {
        qDebug() << "Error opening video stream or file";
    }
}

void VideoThread::run()
{
    while (!m_quit) {
        m_vcap >> m_image;
        emit imageReady(m_image);
    }
}
