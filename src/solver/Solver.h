#ifndef SOLVER_H
#define SOLVER_H

#include "Board.h"
#include "Direction.h"
#include "Point.h"
#include <vector>

class Solver
{
private:
    Board m_board;
    std::vector<bool> m_locked;
    std::vector<Direction> m_solution;

    std::vector<Point> m_parent;    
    std::vector<Direction> m_dirTaken;
    std::vector<int> m_gScore;

    std::vector<int> m_searchID; 
    int m_currentSearchID = 0;

    void lock(Point p);
    bool isLocked(Point p);
    
    // Core routing algorithms
    bool moveEmptyTo(Point target);
    bool moveTileTo(int tileNum, Point target);
    void applyMove(Direction dir);
    
    // Layer by Layer phases
    bool solveRow(int row, int colStart);
    bool solveCol(int col, int rowStart);
    void solveRemaining(); // Solves the final 2x2 grid

    std::vector<Point> getTilePath(Point start, Point target);
    bool macroShiftEmpty(Point tile, Point targetEmpty);

    void optimizeSolution(Board initialBoard);
public:
    Solver(Board board);
    std::vector<Direction> solve();
};

#endif