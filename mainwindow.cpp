#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QTransform>
#include <QIcon>
#include <canthread.h>
#include <QAbstractSocket>
#include <QMessageBox>
#include <QDebug>
#include "filterdialog.h"
#include "srr208parser.h"
#include "addressdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    canThread = new CANThread(this);
    parser = new SRR208Parser(this);
    connect(parser, SIGNAL(trackMapReady(QMap<int,SRR208TrackT>*)), ui->radarPlotWidget, SLOT(onTrackMapReady(QMap<int,SRR208TrackT>*)));
    connect(canThread, SIGNAL(canData(QByteArray)), parser, SLOT(parse(QByteArray)));
    //connect(&canThread, SIGNAL(canData(QByteArray)), this, SLOT(onCanData(QByteArray)));
    connect(canThread, SIGNAL(error(int,QString)), this, SLOT(displayError(int,QString)));

    m_videoThread = new VideoThread;
    qRegisterMetaType<cv::Mat>("cv::Mat");
    connect(m_videoThread, SIGNAL(imageReady(cv::Mat)), ui->videoWidget, SLOT(showImage(cv::Mat)));

    m_videoThread->start();

    //ui->videoWidget->showImage();

}

void MainWindow::displayError(int socketError, const QString &message)
{
    switch (socketError) {
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Blocking Fortune Client"),
                                 tr("The host was not found. Please check the "
                                    "host and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Blocking Fortune Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Blocking Fortune Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(message));
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCanData(QByteArray canData)
{
    QByteArray array = canData.split(' ').at(2);
    ui->textEdit->append(array);
}

void MainWindow::on_actionFilter_Setup_triggered()
{
    FilterDialog *dialog = new FilterDialog(this);
    dialog->exec();
}

void MainWindow::on_actionServer_Address_triggered()
{
    AddressDialog *dialog = new AddressDialog(this);
    dialog->exec();

}

void MainWindow::on_actionRADAR_triggered(bool checked)
{
//    if (checked)
//        canThread->connectToCANServer();
//    else

    if (canThread->isRunning()) {
        qDebug() << "running";
        canThread->disconnectFromServer();
    }
    else {
        qDebug() << "not running";
        canThread->connectToCANServer();
    }
}
