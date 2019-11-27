#include "discretisation.h"

Discretisation::Discretisation()
{

}

void Discretisation::set(int nb_elements, Segment seg)
{
    this->nb_elements = nb_elements;
    this->seg = seg;
    this->start = seg.getStart();
    this->stop = seg.getEnd();
}

void Discretisation::calcCoord()
{
    float * coord = new float[3];
    float size = seg.length();
    Point X;
    float norm[3];

    norm[0] = seg.getEnd().getX() - seg.getStart().getX();
    norm[1] = seg.getEnd().getY() - seg.getStart().getY();
    norm[2] = seg.getEnd().getZ() - seg.getStart().getZ();
    for(int i = 1; i <= this->nb_elements; i++)
    {
        //coord[0] = start.getX()*size/i;
        //coord[1] = start.getY()*size/i;
        //coord[2] = start.getZ()*size/i;

        coord[0] = seg.getStart().getX() + i *(norm[0]/nb_elements);
        coord[1] = seg.getStart().getY() + i *(norm[1]/nb_elements);
        coord[2] = seg.getStart().getZ() + i *(norm[2]/nb_elements);
        X.set(coord);
        pts.append(X);
    }
}

int Discretisation::get_size()
{
    return this->nb_elements;
}

QVector<Point> Discretisation::get_vect()
{
    return this->pts;
}
