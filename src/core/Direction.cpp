#include "Direction.h"
#include <iostream>
#include "Random.h"

Direction::Type Direction::getDir() const { return m_dir; }

Direction Direction::getRandom()
{
    int x{};
    do {
        x = Random::get(-2,2);
    } while(!x); 
    
    return Direction{x};
}

Direction Direction::operator-() const
{
    return Direction{-static_cast<int>(m_dir)};
}

bool Direction::operator==(Direction other) const { return m_dir == other.m_dir; }
bool Direction::operator!=(Direction other) const { return m_dir != other.m_dir; }

// NEW: Maps the core directions to 0, 1, 2, 3 for IDA* array bounds
int Direction::asIndex() const {
    switch(m_dir) {
        case up: return 0;
        case right: return 1;
        case down: return 2;
        case left: return 3;
        default: return 0; 
    }
}

char Direction::toChar() const {
    switch(m_dir) {
        case up: return 'U';
        case right: return 'R';
        case down: return 'D';
        case left: return 'L';
        default: return '?'; 
    }
}