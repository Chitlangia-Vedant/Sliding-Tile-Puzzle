#include"Point.h"
#include"Direction.h"
Point::Point (int x,int y):m_xcoord{x},m_ycoord{y}{}
Point::Point():m_xcoord{0},m_ycoord{0}{}
int Point::getXcoord() const
{
    return m_xcoord;
}
int Point::getYcoord() const
{
    return m_ycoord;
}
Point Point::getAdjacentPoint(Direction x) const
{
    switch(x.getDir())
    {
        case Direction::up: return Point{m_xcoord,m_ycoord-1};
        case Direction::right: return Point{m_xcoord+1,m_ycoord}; 
        case Direction::left: return Point{m_xcoord-1,m_ycoord};
        case Direction::down: return Point{m_xcoord,m_ycoord+1};
        default: return *this;
    }
}
bool operator==(const Point& a,const Point& b)
{
    return ((a.m_xcoord==b.m_xcoord)&&(a.m_ycoord==b.m_ycoord));
}
bool operator!=(const Point& a,const Point& b)
{
    return !(a==b);
}
