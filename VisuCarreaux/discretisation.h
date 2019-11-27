#ifndef DISCRETISATION_H
#define DISCRETISATION_H
#include "segment.h"
#include "point.h"
#include <QVector>

class Discretisation
{
public:
    Discretisation();
    void set(int nb_elements, Segment seg);
    void calcCoord();
    int get_size();
    QVector<Point> get_vect();
    QVector<Point> pts;
    int nb_elements;
    Segment seg;
    Point start;
    Point stop;
};

#endif // DISCRETISATION_H
