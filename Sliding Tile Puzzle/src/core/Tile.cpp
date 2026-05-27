#include "Tile.h"
#include<iostream>
Tile::Tile(int x):m_tile{x}{}

bool Tile::isEmpty() const
{
    return (m_tile==0);
}
int Tile::getNum() const
{
    return (m_tile);
}
