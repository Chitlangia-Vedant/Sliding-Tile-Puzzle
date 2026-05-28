#ifndef TILE_H
#define TILE_H
#include <iostream>

class Tile
{
private:
    int m_tile{};

public:
    Tile(int x);
    
    bool isEmpty() const;
    [[nodiscard]] int getNum() const { return m_tile; }
    
};
#endif
