#include "Board.h"
#include "Logger.h" // Include the logger
#include <iostream>
#include <cassert>

constexpr int g_consoleLines{ 25 };

// Dynamically generate the board based on the requested size
Board::Board(int size) : m_width{size}, m_height{size}, m_empty{size - 1, size - 1} 
{
    int totalTiles{ size * size };
    for (int i{ 1 }; i < totalTiles; ++i) {
        m_board.push_back(Tile{i});
    }
    m_board.push_back(Tile{0}); // The empty tile goes last
}

Board::Board(int width, int height) : m_width{width}, m_height{height}, m_empty{width - 1, height - 1} 
{
    int totalTiles{ width * height };
    for (int i{ 1 }; i < totalTiles; ++i) {
        m_board.push_back(Tile{i});
    }
    m_board.push_back(Tile{0}); // The empty tile goes last
}

std::string Board::getBoardStateString() const {
    std::string state = "[";
    for (size_t i = 0; i < m_board.size(); ++i) {
        state += std::to_string(m_board[i].getNum());
        if (i < m_board.size() - 1) state += ", ";
    }
    state += "]";
    return state;
}

Tile& Board::operator()(int x, int y)
{
    assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
    return m_board[(x) + (y * m_width)];
}

Tile& Board::operator()(Point p)
{
    return (*this)(p.getXcoord(), p.getYcoord());
}

bool Board::moveTile(Direction d, std::string source)
{
    Point p{ m_empty.getAdjacentPoint(-d) };
    
    if ((p.getXcoord() < 0 || p.getXcoord() >= m_width) || 
        (p.getYcoord() < 0 || p.getYcoord() >= m_height))
    {
        return false;
    }
    else    
    {
        // Capture the target coordinate before we overwrite it
        int targetX = m_empty.getXcoord();
        int targetY = m_empty.getYcoord();
        
        Tile temp{ (*this)(m_empty) };
        (*this)(m_empty) = (*this)(p);
        (*this)(p) = temp;
        
        // Find which tile was just moved into the empty space
        int movedTileNum = (*this)(m_empty).getNum();
        m_empty = p;
        
        // Map the Direction enum to a single character
        char dirChar = '?';
        switch(d.getDir()) {
            case Direction::up: dirChar = 'U'; break;
            case Direction::down: dirChar = 'D'; break;
            case Direction::left: dirChar = 'L'; break;
            case Direction::right: dirChar = 'R'; break;
            default: dirChar = '?'; break;
        }

        // Format: [Source] [TileNum]->(x,y): D
        Logger::debug("[" + source + "] [" + std::to_string(movedTileNum) + "]->(" + std::to_string(targetX) + "," + std::to_string(targetY) + "): " + dirChar);
        
        return true;
    }
}

void Board::random(int x)
{
    // Log the initial state before modifying the board
    Logger::info("Board Process: Starting shuffle. Initial state: " + getBoardStateString());
    
    for(int i{0}; i < x; ) 
    {
        Direction dir{};
        // Pass "Shuffle" so the logger knows exactly what is driving the move
        if (moveTile(dir, "Shuffle")) { 
            i++; 
        }
    }
    
    // Log the final scrambled state
    Logger::info("Board Process: Shuffle sequence complete. Final state: " + getBoardStateString());
}

bool Board::solved() const
{
    // Use m_width * m_height - 1 to check all tiles except the empty one
    for(int i{0}; i < (m_width * m_height) - 1; i++)
    {
        if(m_board[i].getNum() != i + 1)
            return false;
    }
    return true;
}


int Board::getWidth() const {
    return m_width;
}

int Board::getHeight() const {
    return m_height;
}

Point Board::getEmptyPoint() const {
    return m_empty;
}

int Board::getTileNum(int x, int y) const {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) return -1;
    return m_board[x + y * m_width].getNum();
}

int Board::getTileNum(Point p) const {
    return getTileNum(p.getXcoord(), p.getYcoord());
}

Point Board::findTile(int num) const {
    for (int y{ 0 }; y < m_height; ++y) {
        for (int x{ 0 }; x < m_width; ++x) {
            if (getTileNum(x, y) == num) {
                return Point{x, y};
            }
        }
    }
    return Point{-1, -1};
}

std::vector<int> Board::toSolverGrid() const {
    std::vector<int> solverGrid;
    solverGrid.reserve(m_width * m_height);
    
    for (size_t i = 0; i < m_board.size(); ++i) {
        solverGrid.push_back(m_board[i].getNum());
    }
    
    return solverGrid;
}