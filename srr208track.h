#ifndef SRR208TRACK_H
#define SRR208TRACK_H

typedef struct {

    bool valid;
    quint16 index;
    quint16 id;
    float RCSValue, RCSValueLast, RCSSum, RCSSumOfSq;
    float longDispl, longDisplLast;
    float latDispl, latDisplLast;
    float vRelLong, vRelLongLast;
    float vRelLat, vRelLatLast;
    float lifeTime, lifeTimeLast;
    ///
    float longDisplAcc;
    float latDisplAcc;
    int rollCount;
    int n;

} SRR208TrackT;

#endif // SRR208TRACK_H

