#ifndef SRR208PARSER_H
#define SRR208PARSER_H
#include <QObject>
#include <QVector>
#include <QMap>
#include "srr208track.h"

class SRR208Parser : public QObject
{
    Q_OBJECT
public:
    explicit SRR208Parser(QObject *parent = 0);
    ~SRR208Parser();

signals:
    void trackMapReady(QMap<int, SRR208TrackT> *trackMap);
    void trackCount(int count);

public slots:
    void parse(QByteArray frame);

private:
    QMap<int, SRR208TrackT> *m_trackMap;
    int m_rollCount;

};

#endif // SRR208PARSER_H
