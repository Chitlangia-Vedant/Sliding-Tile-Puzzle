#ifndef POINT_H
#define POINT_H
#include "Direction.h"

class Point
{
private:
    int m_x{};
    int m_y{};
public:
    Point();
    Point(int x, int y);
    
    [[nodiscard]] int getXcoord() const;
    [[nodiscard]] int getYcoord() const;
    
    [[nodiscard]] inline int to1D(int width) const { return m_y * width + m_x; }

    [[nodiscard]] Point getAdjacentPoint(Direction d) const;
    
    [[nodiscard]] bool operator==(Point p) const;
    [[nodiscard]] bool operator!=(Point p) const;
};

#endif