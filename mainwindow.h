#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "canthread.h"
#include "srr208parser.h"
#include "videothread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *cameraScene;
    CANThread *canThread;
    SRR208Parser *parser;
    VideoThread *m_videoThread;


public slots:
    void onCanData(QByteArray candata);
    void displayError(int socketError, const QString &message);

private slots:
    void on_actionFilter_Setup_triggered();
    void on_actionServer_Address_triggered();
    void on_actionRADAR_triggered(bool checked);
};

#endif // MAINWINDOW_H
