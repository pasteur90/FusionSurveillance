#ifndef CANTHREAD_H
#define CANTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QByteArray>

class CANThread : public QThread
{
    Q_OBJECT
public:
    CANThread(QObject *parent = 0);
    ~CANThread();
    void run() Q_DECL_OVERRIDE;
    void connectToCANServer();
    void disconnectFromServer();

signals:
    void error(int socketError, const QString &message);
    void canData(QByteArray candata);

private:
    QString hostName;
    quint16 port;
    bool quit;
    QMutex mutex;
    QWaitCondition cond;
};

#endif // CANTHREAD_H
