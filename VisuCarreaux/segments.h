#ifndef SEGMENTS_H
#define SEGMENTS_H
#include <QVector>
#include "segment.h"

class Segments
{
public:
    Segments();
    QVector<Segment> vertData;
    void add(Segment s);
    Segment get(int indice);
    void remove(int indice);
    int size();
};

#endif // SEGMENTS_H
