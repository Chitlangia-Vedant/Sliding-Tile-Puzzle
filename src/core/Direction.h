#ifndef DIRECTION_H
#define DIRECTION_H
#include<iostream>
#include<string>
#include<string_view>
class Direction
{
public:
    enum Type
    {
        up=-2,
        right=-1,
        x=0,
        left=1,
        down=2,
    };
private:
    Type m_dir;
public:
    Direction();
    Direction(Type x);
    Direction(int x);
    Type getDir();
    Direction random();
    Direction operator-();

    bool operator==(Direction other) const;
    bool operator!=(Direction other) const;
};

#endif
