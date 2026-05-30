#include"Point.h"
#include"Direction.h"
Point::Point (int x,int y):m_x{x},m_y{y}{}
Point::Point():m_x{0},m_y{0}{}
int Point::getXcoord() const
{
    return m_x;
}
int Point::getYcoord() const
{
    return m_y;
}
Point Point::getAdjacentPoint(Direction x) const
{
    switch(x.getDir())
    {
        case Direction::up: return Point{m_x,m_y-1};
        case Direction::right: return Point{m_x+1,m_y}; 
        case Direction::left: return Point{m_x-1,m_y};
        case Direction::down: return Point{m_x,m_y+1};
        default: return *this;
    }
}
bool Point::operator==(const Point b) const
{
    return ((m_x==b.m_x)&&(m_y==b.m_y));
}
bool Point::operator!=(const Point b) const
{
    return !((*this==b));
}
