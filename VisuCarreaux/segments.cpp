#include "segments.h"
#include <QDebug>
Segments::Segments()
{
    qDebug()<<"creation du buffer de segment";
}

void Segments::add(Segment s)
{
    this->vertData.append(s);
}

Segment Segments::get(int indice)
{
    return this->vertData.at(indice);
}

void Segments::remove(int indice)
{
    this->vertData.remove(indice);
}

int Segments::size()
{
    return this->vertData.size();
}
