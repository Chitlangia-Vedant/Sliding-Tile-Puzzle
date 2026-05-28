#include "Board.h"
#include "Logger.h" // Include the logger
#include <iostream>
#include <sstream>
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
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < m_board.size(); ++i) {
        oss << m_board[i].getNum();
        if (i < m_board.size() - 1) oss << ", ";
    }
    oss << "]";
    return oss.str();
}

uint64_t Board::getFastHash() const {
    uint64_t hash = 0;
    // Safely cap at 16 tiles (64 bits total) to prevent overflow on larger boards
    size_t limit = std::min(m_board.size(), static_cast<size_t>(16));
    for (size_t i = 0; i < limit; ++i) {
        // Shift hash left by 4 bits and append the 4-bit tile number (0-15)
        hash = (hash << 4) | (m_board[i].getNum() & 0xF);
    }
    return hash;
}

Tile& Board::operator()(int x, int y)
{
    assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
    return m_board[(x) + (y * m_width)];
}

Tile& Board::operator()(Point p)
{
    return m_board[p.to1D(m_width)];
}

bool Board::moveTile(Direction d)
{
    Point p{ m_empty.getAdjacentPoint(-d) };
    
    if ((p.getXcoord() < 0 || p.getXcoord() >= m_width) || 
        (p.getYcoord() < 0 || p.getYcoord() >= m_height))
    {
        return false;
    }
    
    Tile temp{ (*this)(m_empty) };
    (*this)(m_empty) = (*this)(p);
    (*this)(p) = temp;
    
    m_empty = p;
    return true; // No more logging or strings here!
}

void Board::random(int x)
{
    std::ostringstream startLog;
    startLog << "Board Process: Starting shuffle. Initial state: " << getBoardStateString();
    Logger::info(startLog.str());
    
    std::ostringstream sequence; // Use a stream instead of a string
    
    for(int i{0}; i < x; ) 
    {
        Direction dir = Direction::getRandom();
        
        if (moveTile(dir)) { 
            if (i > 0) sequence << ", ";
            sequence << dir.toChar();
            i++; 
        }
    }
    
    std::ostringstream seqLog;
    seqLog << "Board Process: Shuffle sequence: [" << sequence.str() << "]";
    Logger::info(seqLog.str());
    
    std::ostringstream endLog;
    endLog << "Board Process: Shuffle sequence complete. Final state: " << getBoardStateString();
    Logger::info(endLog.str());
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

int Board::getTileNum(Point p) const
{
    return m_board[p.to1D(m_width)].getNum();
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