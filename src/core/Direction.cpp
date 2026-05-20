#include "Direction.h"
#include<iostream>
#include "Random.h"
Direction::Direction()
{
    int x{};
    do{
    x=Random::get(-2,2);
    }while(!x);
    m_dir=static_cast<Type>(x);
}
Direction::Direction(Type x):m_dir{x}{}
Direction::Direction(int x):m_dir{static_cast<Type>(x)}{}
Direction::Type Direction::getDir()
{
    return m_dir;
}
Direction Direction::random()
{
    int x{};
    do{
    x=Random::get(-2,2);
    }while(!x);
    return Direction{x};
}
Direction Direction::operator-()
{
    return Direction{-static_cast<int>(m_dir)};
}
bool Direction::operator==(Direction other) const
{
    return m_dir == other.m_dir;
}

bool Direction::operator!=(Direction other) const
{
    return m_dir != other.m_dir;
}