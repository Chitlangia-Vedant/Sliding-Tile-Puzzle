#ifndef POINT_H
#define POINT_H
#include "Direction.h"
class Point
{
private:
    int m_xcoord;
    int m_ycoord;
public:
    Point(int x,int y);
    Point(); 
    int getXcoord() const;
    int getYcoord() const;
    Point getAdjacentPoint(Direction x) const;
    friend bool operator==(const Point& a,const Point& b);
    friend bool operator!=(const Point& a,const Point& b);
};
#endif
