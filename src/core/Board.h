#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <vector>
#include <string> // NEW: Required for string manipulation
#include "Tile.h"
#include "Direction.h"
#include "Point.h"

class Board
{
private:
    int m_width{};  // Replaced m_size
    int m_height{}; // Replaced m_size
    std::vector<Tile> m_board{};
    Point m_empty{};
public:
    Board(int size); // New constructor
    Board(int width, int height);
    Tile& operator()(int x, int y);
    Tile& operator()(Point p);
    bool moveTile(Direction d, std::string source = "Manual");
    void random(int x);
    bool solved() const;
    int getWidth() const;
    int getHeight() const;
    Point getEmptyPoint() const;
    int getTileNum(int x, int y) const;
    int getTileNum(Point p) const;
    Point findTile(int num) const;
    std::string getBoardStateString() const;
    std::vector<int> toSolverGrid() const;
};
#endif