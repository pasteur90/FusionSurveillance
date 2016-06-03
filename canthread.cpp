#include "canthread.h"

#include <QtNetwork>
#include <QDebug>
#include <QByteArray>
#include <QSettings>

CANThread::CANThread(QObject *parent)
    : QThread(parent), quit(false)
{
            qDebug() << "constructor";
}

CANThread::~CANThread()
{
    qDebug() << "destructor";
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}

void CANThread::connectToCANServer()
{
    QMutexLocker locker(&mutex);

    QSettings settings;

    QString serverName = settings.value("net/radarAddress").toString();
    quint16 serverPort = settings.value("net/radarPort").toUInt();

    this->hostName = serverName;
    this->port = serverPort;
    if (!isRunning()) {
        quit = false;
        start();
        qDebug() << "start";
    }
    /*
    else
        cond.wakeOne();
     */
}

void CANThread::disconnectFromServer()
{
    this->quit = true;
}

void CANThread::run()
{

    QTcpSocket socket;
    QByteArray buffer;
    socket.connectToHost(hostName, port);

    if (!socket.waitForConnected(5000)) {
        emit error(socket.error(), socket.errorString());
        return;
    }

    while (!quit) {
        if (!socket.waitForReadyRead(3000)) {
            emit error(socket.error(), socket.errorString());
            return;
        }

        buffer.append(socket.readAll());
        static const QByteArrayMatcher endSequenceMatcher(QByteArray("\n"));
        QByteArray frame;
        int endSequencePos;

        while (buffer.size() > 0) {
            endSequencePos = endSequenceMatcher.indexIn(buffer);
            if (endSequencePos == -1) {
                break;
            }
            else {
                frame.append(buffer.data(), endSequencePos);
                buffer.remove(0, endSequencePos + 1);
                emit canData(frame);
                frame.clear();
            }
        }
    }

    socket.close();
}
