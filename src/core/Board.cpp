#include "Board.h"
#include "Logger.h" // Include the logger
#include <iostream>
#include <sstream>
#include <cassert>
#include <cmath>


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

int Board::moveTileAt(int x, int y)
{
    // 1. Initial bounds check
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return 0; // 0 represents Direction::x (no movement)
    }

    // 2. Determine spatial relationship
    int dx = m_empty.getXcoord() - x;
    int dy = m_empty.getYcoord() - y;

    int moveDir = 0; // Default to no movement

    // 3. Map adjacent positions to their integer directions
    if (dx == 1 && dy == 0) {
        moveDir = Direction::right; // -1
    } else if (dx == -1 && dy == 0) {
        moveDir = Direction::left;  // 1
    } else if (dx == 0 && dy == 1) {
        moveDir = Direction::down;  // 2
    } else if (dx == 0 && dy == -1) {
        moveDir = Direction::up;    // -2
    }

    // 4. If adjacent (moveDir is not 0), swap tiles
    if (moveDir != 0) {
        Point target{x, y};
        
        // Swap directly instead of calling moveTile!
        Tile temp{ (*this)(m_empty) };
        (*this)(m_empty) = (*this)(target);
        (*this)(target) = temp;
        
        m_empty = target;
        
        return moveDir; 
    }

    return 0; // Not adjacent, return 0
}

int Board::moveTileNumber(int tile)
{
    // Ensure the tile is a valid numbered tile
    if (tile <= 0 || tile >= m_width * m_height) {
        return 0; 
    }

    // Locate the tile
    Point tilePoint = findTile(tile);

    // Ensure the tile was actually found
    if (tilePoint.getXcoord() < 0 || tilePoint.getYcoord() < 0) {
        return 0;
    }

    // Delegate to moveTileAt and return the direction it yields
    return moveTileAt(tilePoint.getXcoord(), tilePoint.getYcoord());
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

bool Board::isTileCorrect(int x, int y) const
{
    // Bounds check
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return false;
    }

    int index = x + y * m_width;
    
    // The expected tile number is (index + 1), 
    // UNLESS it's the very last space, which expects the empty tile (0)
    int expectedTile = (index == m_width * m_height - 1) ? 0 : (index + 1);
    
    return m_board[index].getNum() == expectedTile;
}

bool Board::isTileCorrect(int t) const
{
    // Ensure the requested tile is a valid number
    if (t < 0 || t >= m_width * m_height) {
        return false;
    }

    // Where should this tile be? 
    // Tile 0 belongs at the end. Tiles 1 to N belong at (t - 1).
    int expectedIndex = (t == 0) ? (m_width * m_height - 1) : (t - 1);
    
    return m_board[expectedIndex].getNum() == t;
}

std::vector<int> Board::getCorrectTiles() const
{
    std::vector<int> correctTiles;
    int totalSpaces = m_width * m_height;
    
    correctTiles.reserve(totalSpaces); // Pre-allocate for minor optimization

    for (int i = 0; i < totalSpaces; ++i) {
        int expectedTile = (i == totalSpaces - 1) ? 0 : (i + 1);
        
        if (m_board[i].getNum() == expectedTile) {
            correctTiles.push_back(expectedTile);
        }
    }
    
    return correctTiles;
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

bool Board::setGrid(const std::vector<int>& grid) {
    const int expectedSize = m_width * m_height;
    if (grid.size() != static_cast<size_t>(expectedSize)) return false;

    std::vector<bool> seen(expectedSize, false);
    int emptyIndex = -1;

    for (int i = 0; i < expectedSize; ++i) {
        int value = grid[i];
        if (value < 0 || value >= expectedSize || seen[value]) return false;

        seen[value] = true;
        if (value == 0) emptyIndex = i;
    }

    if (emptyIndex < 0) return false;

    m_board.clear();
    m_board.reserve(expectedSize);

    for (int value : grid) {
        m_board.push_back(Tile{value});
    }

    m_empty = Point{emptyIndex % m_width, emptyIndex / m_width};
    return true;
}

bool Board::isSolvable() const {
    // Delegate to the static function to avoid repeating logic
    return isSolvable(toSolverGrid(), m_width, m_height);
}

bool Board::isSolvable(const std::vector<int>& grid, int width, int height) {
    int inversions = 0;
    for (int i = 0; i < static_cast<int>(grid.size()); ++i) {
        for (int j = i + 1; j < static_cast<int>(grid.size()); ++j) {
            if (grid[i] != 0 && grid[j] != 0 && grid[i] > grid[j]) {
                inversions++;
            }
        }
    }

    if (width % 2 == 1) {
        return inversions % 2 == 0;
    }

    int blankIndex = -1;
    for (int i = 0; i < static_cast<int>(grid.size()); ++i) {
        if (grid[i] == 0) {
            blankIndex = i;
            break;
        }
    }

    int blankRowFromBottom = height - (blankIndex / width);

    if (blankRowFromBottom % 2 == 0) {
        return inversions % 2 == 1;
    }

    return inversions % 2 == 0;
}