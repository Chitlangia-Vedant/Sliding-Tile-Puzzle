#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <vector>
#include <string> // NEW: Required for string manipulation
#include <cstdint>
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
    bool moveTile(Direction d);
    void random(int x);
    [[nodiscard]] bool solved() const;
    [[nodiscard]] int getWidth() const { return m_width; }
    [[nodiscard]] int getHeight() const { return m_height; }
    [[nodiscard]] Point getEmptyPoint() const { return m_empty; }
    [[nodiscard]] int getTileNum(int x, int y) const;
    [[nodiscard]] int getTileNum(Point p) const;
    Point findTile(int num) const;
    [[nodiscard]] std::string getBoardStateString() const;
    [[nodiscard]] uint64_t getFastHash() const;
    [[nodiscard]] std::vector<int> toSolverGrid() const;
};
#endif