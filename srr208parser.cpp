#include "srr208parser.h"
#include <QDebug>
#include <QVector>

SRR208Parser::SRR208Parser(QObject *parent) :
    QObject(parent),
    m_trackMap(new QMap<int, SRR208TrackT>),
    m_rollCount(0)
{
    qDebug() << "parser";
}

SRR208Parser::~SRR208Parser()
{

}

void SRR208Parser::parse(QByteArray canData)
{
    QByteArray frame = canData.split(' ').at(2);
    //remove the character which indicates message type
    int id = frame.mid(0, 3).toInt(0, 16);

    //remove id, dlc from this frame
    frame.remove(0, 4);

    static int numTrack = 0;
    //0x60c and 0x60d shares track and trackId (differ from local var id);
    static SRR208TrackT track;
    static int trackId;

    switch (id)
    {
    case 0x60b:
        numTrack = frame.mid(0, 2).toInt(0, 16);
        emit trackCount(numTrack);
        break;

    case 0x60c:
    {
        if (numTrack == 0)
            break;

        track.latDisplAcc = track.latDisplLast = track.lifeTimeLast = track.longDisplAcc = track.longDisplLast = 0.0;
        track.RCSSum = track.RCSSumOfSq = track.RCSValueLast = track.vRelLatLast = track.vRelLongLast = 0.0;
        track.n = 1;

        track.id = trackId = frame.mid(0, 4).toInt(0, 16);
        track.longDispl = ((frame.mid(4, 2).toInt(0, 16) << 3 ) | (frame.mid(6, 1).toInt(0, 16) >> 1)) * 0.1;
        track.index = (frame.mid(6, 2).toInt(0, 16) & 0x1f);
        track.latDispl = ((frame.mid(8, 2).toInt(0, 16) << 2) | (frame.mid(10, 1).toInt(0, 16) >> 2)) * 0.1f - 51.1f;
        track.vRelLong = (((frame.mid(10, 2).toInt(0, 16) & 0x3f) << 6) | (frame.mid(12, 2).toInt(0, 16) >> 2)) * 0.02f - 35.0f;
        track.vRelLat = ((frame.mid(14, 2).toInt(0, 16))) * 0.25f - 32.0f;
        //the latest value of rollCount will prevent the track from removal
        track.rollCount = m_rollCount;

        break;
    }

    case 0x60d:
    {
        //TODO: must be aware of the situation that the first frame acquired is 0x60d
        if (numTrack == 0)
            break;
//        if (trackVector == NULL) //segmentation fault prevention (null pointer dereferencing)
//            break;
//        if (trackVector->isEmpty())
//            break;
        int _index = frame.mid(6, 2).toInt(0, 16) & 0x1f;

        //something went wrong
        if (_index != track.index)
            break;

        track.lifeTime = frame.mid(2, 4).toInt(0, 16) * 0.1f;
        track.RCSValue = frame.mid(0, 2).toInt(0, 16) * 0.5 - 50.0f;

        if (m_trackMap->contains(trackId)) {
            const SRR208TrackT &_track = m_trackMap->value(trackId);
            track.longDisplLast = _track.longDispl;
            track.latDisplLast = _track.latDispl;
            track.vRelLongLast = _track.vRelLong;
            track.vRelLatLast = _track.vRelLat;
            track.lifeTimeLast = _track.lifeTime;
            track.latDisplAcc = _track.latDisplAcc + track.latDispl - track.latDisplLast;
            track.longDisplAcc = _track.longDisplAcc + track.longDispl - track.longDisplLast;
            track.RCSValueLast = _track.RCSValue;
            if (track.RCSValue > -49.0) {
                track.n = _track.n + 1;
                track.RCSSum = _track.RCSSum + track.RCSValue;
                track.RCSSumOfSq = _track.RCSSumOfSq + track.RCSValue * track.RCSValue;
            }
            else {
                track.n = _track.n;
                track.RCSSum = _track.RCSSum;
                track.RCSSumOfSq = _track.RCSSumOfSq;
            }
            m_trackMap->insert(trackId, track);
        }
        else {
            track.longDisplLast = 0.0;
            track.longDisplAcc = 0.0;
            track.latDisplLast = 0.0;
            track.latDisplAcc = 0.0;
            track.vRelLatLast = 0.0;
            track.vRelLongLast = 0.0;
            track.RCSValueLast = 0.0;
            track.lifeTimeLast = 0.0;
            track.RCSSum = (track.RCSValue > -49.0 ? track.RCSValue : 0.0);
            track.RCSSumOfSq = (track.RCSValue > -49.0 ? track.RCSValue : 0.0)
                    * (track.RCSValue > -49.0 ? track.RCSValue : 0.0);
            track.RCSValueLast = 0.0;
            m_trackMap->insert(trackId, track);
        }

        if (_index == numTrack - 1) {
            //remove outdated tracks (those that don't exist no more)
            QMap<int, SRR208TrackT>::iterator i = m_trackMap->begin();
            while (i != m_trackMap->end()) {
                SRR208TrackT &__track = i.value();
                //qDebug() << "__track: " << __track.rollCount << " m_rollCount: " << m_rollCount;
                if (__track.rollCount != m_rollCount) {
                    i = m_trackMap->erase(i);
                }
                else
                    ++i;
            }

            //TODO: inform trackplot
            emit trackMapReady(m_trackMap);
            m_rollCount = (m_rollCount + 1);
        }

        break;
    }


    }

}
