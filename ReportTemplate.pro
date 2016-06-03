#-------------------------------------------------
#
# Project created by QtCreator 2016-02-04T17:44:32
#
#-------------------------------------------------

QT       += core gui widgets network
CONFIG   += qwt link_pkgconfig
PKGCONFIG+= opencv


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ReportTemplate
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    radarplot.cpp \
    curvedata.cpp \
    lidarplot.cpp \
    canthread.cpp \
    srr208parser.cpp \
    filterdialog.cpp \
    addressdialog.cpp \
    videothread.cpp

HEADERS  += mainwindow.h \
    radarplot.h \
    curvedata.h \
    lidarplot.h \
    canthread.h \
    srr208parser.h \
    srr208track.h \
    filterdialog.h \
    addressdialog.h \
    cvimagewidget.h \
    videothread.h

FORMS    += mainwindow.ui \
    filterdialog.ui \
    addressdialog.ui

RESOURCES +=
