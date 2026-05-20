#include "Solver.h"
#include "../core/Logger.h"
#include <queue>
#include <map>
#include <algorithm>

Solver::Solver(Board board) : m_board{board} 
{
    int width = m_board.getWidth();
    int height = m_board.getHeight();
    int totalTiles = width * height;
    
    // Initialize the locked grid to match height (rows) and width (columns)
    m_locked.resize(height, std::vector<bool>(width, false));
    
    // Pre-allocate the arrays for the maximum possible board space
    m_parent.resize(totalTiles, Point{-1, -1});
    m_dirTaken.resize(totalTiles, Direction::x);
    m_gScore.resize(totalTiles, 999999); 
    m_searchID.resize(totalTiles, 0);
}

void Solver::lock(Point p) {
    m_locked[p.getYcoord()][p.getXcoord()] = true;
}

bool Solver::isLocked(Point p) const {
    if (p.getXcoord() < 0 || p.getXcoord() >= m_board.getWidth() ||
        p.getYcoord() < 0 || p.getYcoord() >= m_board.getHeight()) return true; 
    return m_locked[p.getYcoord()][p.getXcoord()];
}

void Solver::applyMove(Direction dir) {
    if (m_board.moveTile(dir, "AutoSolver")) {
        m_solution.push_back(dir);
    }
}

bool Solver::moveEmptyTo(Point target) {
    Point start = m_board.getEmptyPoint();
    if (start == target) return true;

    int width = m_board.getWidth();
    m_currentSearchID++;

    struct AStarNode {
        int f; 
        Point p;
        bool operator>(const AStarNode& other) const { return f > other.f; } 
    };

    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> pq;
    
    int startIdx = start.getYcoord() * width + start.getXcoord();
    
    m_searchID[startIdx] = m_currentSearchID;
    m_parent[startIdx] = start;
    m_gScore[startIdx] = 0;
    
    int initial_h = std::abs(start.getXcoord() - target.getXcoord()) + std::abs(start.getYcoord() - target.getYcoord());
    pq.push({initial_h, start});

    Direction dirs[] = {Direction::up, Direction::down, Direction::left, Direction::right};

    while (!pq.empty()) {
        Point curr = pq.top().p;
        pq.pop();

        if (curr == target) break;

        int currIdx = curr.getYcoord() * width + curr.getXcoord();

        for (Direction d : dirs) {
            Point next = curr.getAdjacentPoint(-d); 
            
            if (!isLocked(next)) {
                int nextIdx = next.getYcoord() * width + next.getXcoord();
                int tentative_g = m_gScore[currIdx] + 1; 
                
                if (m_searchID[nextIdx] != m_currentSearchID || tentative_g < m_gScore[nextIdx]) {
                    
                    m_searchID[nextIdx] = m_currentSearchID; 
                    m_parent[nextIdx] = curr;
                    m_dirTaken[nextIdx] = d;
                    m_gScore[nextIdx] = tentative_g;
                    
                    int h = std::abs(next.getXcoord() - target.getXcoord()) + std::abs(next.getYcoord() - target.getYcoord());
                    pq.push({tentative_g + h, next});
                }
            }
        }
    }

    int targetIdx = target.getYcoord() * width + target.getXcoord();
    if (m_searchID[targetIdx] != m_currentSearchID) return false; 

    std::vector<Direction> path;
    Point curr = target;
    while (curr != start) {
        int currIdx = curr.getYcoord() * width + curr.getXcoord();
        path.push_back(m_dirTaken[currIdx]);
        curr = m_parent[currIdx]; 
    }
    
    std::reverse(path.begin(), path.end());
    for (Direction d : path) {
        applyMove(d);
    }
    return true;
}

bool Solver::moveTileTo(int tileNum, Point target) {
    Point current = m_board.findTile(tileNum);
    
    while (current != target) {
        std::vector<Point> tilePath = getTilePath(current, target);
        if (tilePath.empty()) {
            Logger::warn("Pathfinding failed: Tile trapped.");
            return false;
        }
        
        Point nextTilePos = tilePath[0];
        
        int dx = nextTilePos.getXcoord() - current.getXcoord();
        int dy = nextTilePos.getYcoord() - current.getYcoord();
        Direction tileMove = Direction::x;
        
        if (dx > 0) tileMove = Direction::right;
        else if (dx < 0) tileMove = Direction::left;
        else if (dy > 0) tileMove = Direction::down;
        else if (dy < 0) tileMove = Direction::up;

        Point requiredEmptyPos = nextTilePos;
        Point emptyPos = m_board.getEmptyPoint();

        lock(current); 
        
        bool emptyMoved = true; 
        
        if (emptyPos != requiredEmptyPos) {
            if (abs(emptyPos.getXcoord() - current.getXcoord()) + abs(emptyPos.getYcoord() - current.getYcoord()) == 1) {
                if (!macroShiftEmpty(current, requiredEmptyPos)) {
                    emptyMoved = moveEmptyTo(requiredEmptyPos); 
                }
            } else {
                emptyMoved = moveEmptyTo(requiredEmptyPos); 
            }
        }
        
        m_locked[current.getYcoord()][current.getXcoord()] = false; 

        if (!emptyMoved) {
            return false; 
        }     
        applyMove(tileMove); 
        current = m_board.findTile(tileNum); 
    }
    return true;
}

bool Solver::solveRow(int row, int colStart) {
    int width = m_board.getWidth();
    int height = m_board.getHeight();
    
    for (int col = colStart; col < width - 2; ++col) {
        int targetNum = row * width + col + 1;
        if (!moveTileTo(targetNum, Point{col, row})) return false;
        lock(Point{col, row});
    }

    int targetNum1 = row * width + (width - 1); 
    int targetNum2 = row * width + width;       

    bool flushed = false; 

    while (true) {
        Board boardBackup = m_board;
        auto lockedBackup = m_locked;
        auto solutionBackup = m_solution;

        // --- Method 1 (Type 1 Logic) ---
        bool method1Success = true;
        if (!moveTileTo(targetNum2, Point{width - 2, row})) method1Success = false;
        if (method1Success) {
            lock(Point{width - 2, row});
            if (!moveTileTo(targetNum1, Point{width - 2, row + 1})) method1Success = false;
        }
        if (method1Success) {
            lock(Point{width - 2, row + 1});
            if (!moveEmptyTo(Point{width - 1, row + 1})) method1Success = false;
        }
        if (method1Success) {
            m_locked[row][width - 2] = false;
            m_locked[row + 1][width - 2] = false;

            applyMove(Direction::down);
            applyMove(Direction::right);
            applyMove(Direction::up);
        }
        int method1Moves = method1Success ? m_solution.size() - solutionBackup.size() : 999999;
        
        Board method1Board = m_board;
        auto method1Locked = m_locked;
        auto method1Solution = m_solution;

        m_board = boardBackup;
        m_locked = lockedBackup;
        m_solution = solutionBackup;

        // --- Method 2 (Type 2 Logic) ---
        bool method2Success = true;
        if (!moveTileTo(targetNum1, Point{width - 1, row})) method2Success = false;
        if (method2Success) {
            lock(Point{width - 1, row});
            if (!moveTileTo(targetNum2, Point{width - 1, row + 1})) method2Success = false;
        }
        if (method2Success) {
            lock(Point{width - 1, row + 1});
            if (!moveEmptyTo(Point{width - 2, row + 1})) method2Success = false;
        }
        if (method2Success) {
            m_locked[row][width - 1] = false;
            m_locked[row + 1][width - 1] = false;

            applyMove(Direction::down);
            applyMove(Direction::left);
            applyMove(Direction::up);
        }
        int method2Moves = method2Success ? m_solution.size() - solutionBackup.size() : 999999;

        if (method1Success || method2Success) {
            if (method1Moves < method2Moves) {
                m_board = method1Board;
                m_locked = method1Locked;
                m_solution = method1Solution;
            }
            break; 
        }

        if (flushed) {
            Logger::warn("Both row macros failed even after flushing out the dead-end.");
            return false; 
        }

        Logger::info("Both row macros failed. Flushing tiles out into the active grid...");
        m_board = boardBackup;
        m_locked = lockedBackup;
        m_solution = solutionBackup;

        int stageY = std::min(row + 2, height - 1); 
        if (!moveTileTo(targetNum1, Point{width - 1, stageY})) return false;
        if (!moveTileTo(targetNum2, Point{width - 2, stageY})) return false;

        flushed = true; 
    }

    lock(Point{width - 2, row});
    lock(Point{width - 1, row});
    return true;
}

bool Solver::solveCol(int col, int rowStart) {
    int width = m_board.getWidth();
    int height = m_board.getHeight();
    
    for (int row = rowStart; row < height - 2; ++row) {
        int targetNum = row * width + col + 1;
        if (!moveTileTo(targetNum, Point{col, row})) return false;
        lock(Point{col, row});
    }

    int targetNum1 = (height - 2) * width + col + 1; 
    int targetNum2 = (height - 1) * width + col + 1; 

    bool flushed = false; 

    while (true) {
        Board boardBackup = m_board;
        auto lockedBackup = m_locked;
        auto solutionBackup = m_solution;

        // --- Method 1 (Type 1 Logic rotated) ---
        bool method1Success = true;
        if (!moveTileTo(targetNum2, Point{col, height - 2})) method1Success = false;
        if (method1Success) {
            lock(Point{col, height - 2});
            if (!moveTileTo(targetNum1, Point{col + 1, height - 2})) method1Success = false;
        }
        if (method1Success) {
            lock(Point{col + 1, height - 2});
            if (!moveEmptyTo(Point{col + 1, height - 1})) method1Success = false;
        }
        if (method1Success) {
            m_locked[height - 2][col] = false;
            m_locked[height - 2][col + 1] = false;

            applyMove(Direction::right); 
            applyMove(Direction::down);  
            applyMove(Direction::left);  
        }
        int method1Moves = method1Success ? m_solution.size() - solutionBackup.size() : 999999;
        
        Board method1Board = m_board;
        auto method1Locked = m_locked;
        auto method1Solution = m_solution;

        m_board = boardBackup;
        m_locked = lockedBackup;
        m_solution = solutionBackup;

        // --- Method 2 (Type 2 Logic rotated) ---
        bool method2Success = true;
        if (!moveTileTo(targetNum1, Point{col, height - 1})) method2Success = false;
        if (method2Success) {
            lock(Point{col, height - 1});
            if (!moveTileTo(targetNum2, Point{col + 1, height - 1})) method2Success = false;
        }
        if (method2Success) {
            lock(Point{col + 1, height - 1});
            if (!moveEmptyTo(Point{col + 1, height - 2})) method2Success = false;
        }
        if (method2Success) {
            m_locked[height - 1][col] = false;
            m_locked[height - 1][col + 1] = false;

            applyMove(Direction::right); 
            applyMove(Direction::up);    
            applyMove(Direction::left);  
        }
        int method2Moves = method2Success ? m_solution.size() - solutionBackup.size() : 999999;

        if (method1Success || method2Success) {
            if (method1Moves < method2Moves) {
                m_board = method1Board;
                m_locked = method1Locked;
                m_solution = method1Solution;
            }
            break; 
        }

        if (flushed) {
            Logger::warn("Both column macros failed even after flushing out the dead-end.");
            return false; 
        }

        Logger::info("Both column macros failed. Flushing tiles out into the active grid...");
        m_board = boardBackup;
        m_locked = lockedBackup;
        m_solution = solutionBackup;

        int stageX = std::min(col + 2, width - 1); 
        if (!moveTileTo(targetNum1, Point{stageX, height - 1})) return false;
        if (!moveTileTo(targetNum2, Point{stageX, height - 2})) return false;

        flushed = true; 
    }

    lock(Point{col, height - 2});
    lock(Point{col, height - 1});
    return true;
}

void Solver::solveRemaining() {
    int w = m_board.getWidth();
    int h = m_board.getHeight();

    moveEmptyTo(Point{w - 1, h - 1});

    Point p0{w - 2, h - 2};
    Point p1{w - 1, h - 2};
    Point p2{w - 2, h - 1};
    Point p3{w - 1, h - 1};

    int t0 = (h - 2) * w + (w - 2) + 1;
    int t1 = (h - 2) * w + (w - 1) + 1;
    int t2 = (h - 1) * w + (w - 2) + 1;
    int t3 = 0; 

    std::vector<int> goalState = {t0, t1, t2, t3};

    std::vector<int> startState = {
        m_board.getTileNum(p0),
        m_board.getTileNum(p1),
        m_board.getTileNum(p2),
        m_board.getTileNum(p3)
    };

    if (startState == goalState) return; 

    std::map<std::vector<int>, std::vector<int>> parent; 
    std::map<std::vector<int>, Direction> moveTaken;     
    std::queue<std::vector<int>> q;

    q.push(startState);
    parent[startState] = startState; 

    std::vector<std::vector<std::pair<int, Direction>>> transitions = {
        {{1, Direction::left}, {2, Direction::up}},    
        {{0, Direction::right}, {3, Direction::up}},   
        {{0, Direction::down}, {3, Direction::left}},  
        {{1, Direction::down}, {2, Direction::right}}  
    };

    bool found = false;

    while (!q.empty()) {
        auto curr = q.front();
        q.pop();

        if (curr == goalState) {
            found = true;
            break;
        }

        int emptyIdx = 0;
        for (int i = 0; i < 4; ++i) {
            if (curr[i] == 0) {
                emptyIdx = i;
                break;
            }
        }

        for (const auto& transition : transitions[emptyIdx]) {
            int adjIdx = transition.first;
            Direction dir = transition.second;

            std::vector<int> nextState = curr;
            std::swap(nextState[emptyIdx], nextState[adjIdx]);

            if (parent.find(nextState) == parent.end()) {
                parent[nextState] = curr;
                moveTaken[nextState] = dir;
                q.push(nextState);
            }
        }
    }

    if (found) {
        std::vector<Direction> path;
        auto curr = goalState;
        
        while (curr != startState) {
            path.push_back(moveTaken[curr]);
            curr = parent[curr];
        }
        
        std::reverse(path.begin(), path.end());
        for (Direction d : path) {
            applyMove(d);
        }
    } else {
        Logger::warn("Solver Error: Unsolvable parity in the final 2x2 grid.");
    }
}

std::vector<Direction> Solver::solve() {
    Logger::info("[Solver] Initial State: " + m_board.getBoardStateString());
    
    Board initialBoard = m_board;

    int w = m_board.getWidth();
    int h = m_board.getHeight();
    
    int r = 0; // Current row being solved
    int c = 0; // Current column being solved
    
    // Dynamically peel off layers until only a 2x2 remains
    while (h - r > 2 || w - c > 2) {
        if (h - r > 2) {
            solveRow(r, c);
            r++;
        }
        if (w - c > 2) {
            solveCol(c, r);
            c++;
        }
    }
    
    solveRemaining();
    
    int originalMoves = m_solution.size();
    optimizeSolution(initialBoard);
    int optimizedMoves = m_solution.size();
    
    if (originalMoves != optimizedMoves) {
        Logger::info("[Solver] Optimizer compressed " + std::to_string(originalMoves) + 
                     " moves down to " + std::to_string(optimizedMoves) + " moves!");
    }
    
    Logger::info("[Solver] Final State: " + m_board.getBoardStateString());
    return m_solution;
}

std::vector<Point> Solver::getTilePath(Point start, Point target) {
    int width = m_board.getWidth();
    m_currentSearchID++; 

    struct AStarNode {
        int f;
        Point p;
        bool operator>(const AStarNode& other) const { return f > other.f; }
    };
    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> pq;

    int startIdx = start.getYcoord() * width + start.getXcoord();
    
    m_searchID[startIdx] = m_currentSearchID;
    m_parent[startIdx] = start;
    m_gScore[startIdx] = 0;

    int initial_h = std::abs(start.getXcoord() - target.getXcoord()) + std::abs(start.getYcoord() - target.getYcoord());
    pq.push({initial_h, start});

    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    while (!pq.empty()) {
        Point curr = pq.top().p;
        pq.pop();

        if (curr == target) break;
        
        int currIdx = curr.getYcoord() * width + curr.getXcoord();

        for (int i = 0; i < 4; ++i) {
            Point next{curr.getXcoord() + dx[i], curr.getYcoord() + dy[i]};
            
            if (!isLocked(next)) {
                int nextIdx = next.getYcoord() * width + next.getXcoord();
                int tentative_g = m_gScore[currIdx] + 1;
                
                if (m_searchID[nextIdx] != m_currentSearchID || tentative_g < m_gScore[nextIdx]) {
                    
                    m_searchID[nextIdx] = m_currentSearchID;
                    m_parent[nextIdx] = curr;
                    m_gScore[nextIdx] = tentative_g;
                    
                    int h = std::abs(next.getXcoord() - target.getXcoord()) + std::abs(next.getYcoord() - target.getYcoord());
                    pq.push({tentative_g + h, next});
                }
            }
        }
    }
    
    int targetIdx = target.getYcoord() * width + target.getXcoord();
    if (m_searchID[targetIdx] != m_currentSearchID) return {}; 
    
    std::vector<Point> path;
    Point curr = target;
    while (curr != start) {
        path.push_back(curr);
        curr = m_parent[curr.getYcoord() * width + curr.getXcoord()];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

bool Solver::macroShiftEmpty(Point tile, Point targetEmpty) {
    Point startEmpty = m_board.getEmptyPoint();
    if (startEmpty == targetEmpty) return true;

    int width = m_board.getWidth();
    m_currentSearchID++; 

    std::queue<Point> q;
    q.push(startEmpty);
    
    int startIdx = startEmpty.getYcoord() * width + startEmpty.getXcoord();
    m_searchID[startIdx] = m_currentSearchID;
    m_parent[startIdx] = startEmpty;

    Direction dirs[] = {Direction::up, Direction::down, Direction::left, Direction::right};

    while (!q.empty()) {
        Point curr = q.front();
        q.pop();

        if (curr == targetEmpty) break;

        for (Direction d : dirs) {
            Point next = curr.getAdjacentPoint(-d); 
            
            if (!isLocked(next) && next != tile && 
                abs(next.getXcoord() - tile.getXcoord()) <= 1 && 
                abs(next.getYcoord() - tile.getYcoord()) <= 1) 
            {
                int nextIdx = next.getYcoord() * width + next.getXcoord();
                
                if (m_searchID[nextIdx] != m_currentSearchID) {
                    m_searchID[nextIdx] = m_currentSearchID; 
                    m_parent[nextIdx] = curr;
                    m_dirTaken[nextIdx] = d;
                    q.push(next);
                }
            }
        }
    }

    int targetIdx = targetEmpty.getYcoord() * width + targetEmpty.getXcoord();
    if (m_searchID[targetIdx] != m_currentSearchID) return false; 

    std::vector<Direction> path;
    Point curr = targetEmpty;
    while (curr != startEmpty) {
        int currIdx = curr.getYcoord() * width + curr.getXcoord();
        path.push_back(m_dirTaken[currIdx]);
        curr = m_parent[currIdx];
    }
    std::reverse(path.begin(), path.end());
    for (Direction d : path) applyMove(d);
    
    return true;
}

void Solver::optimizeSolution(Board startBoard) {
    bool changed = true;
    
    while (changed) {
        changed = false;

        std::vector<Direction> stack;
        for (Direction d : m_solution) {
            if (!stack.empty() && stack.back() == -d) 
            {
                stack.pop_back(); 
                changed = true;
            } else {
                stack.push_back(d);
            }
        }
        m_solution = stack;

        std::map<std::string, int> stateToIndex;
        std::vector<Direction> loopFree;
        Board simBoard = startBoard;
        
        stateToIndex[simBoard.getBoardStateString()] = 0;
        
        for (Direction d : m_solution) {
            simBoard.moveTile(d, "Optimizer"); 
            loopFree.push_back(d);
            
            std::string stateStr = simBoard.getBoardStateString();
            
            if (stateToIndex.find(stateStr) != stateToIndex.end()) {
                int loopStart = stateToIndex[stateStr];
                loopFree.resize(loopStart); 
                
                m_solution = loopFree;
                changed = true;
                break; 
            } else {
                stateToIndex[stateStr] = loopFree.size();
            }
        }
    }
}