#include "IdaBridge.h"

// Now it is safe to include the IDA headers because they are in the 'ida' namespace
#include "../IDAsolver/include/SearchNode.h"      // NEW
#include "../IDAsolver/include/SearchNodeRect.h"
#include "../IDAsolver/include/DisjointDatabase.h"
#include "../IDAsolver/include/WalkingDistance.h"
#include "../IDAsolver/include/Idastar.h"
#include "../core/Logger.h"
#include "../IDAsolver/include/Util.h"
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace ida; // Use the namespace you added in Step 1

// Copied from your old wasm.cpp to ensure solvable check still happens
int getInversions(const std::vector<int>& board) {
    int inversions = 0;
    for (int i = 0; i < board.size(); i++) {
        for (int j = i + 1; j < board.size(); j++) {
            if (board[i] != 0 && board[j] != 0 && board[i] > board[j]) {
                inversions++;
            }
        }
    }
    return inversions;
}

bool solvable(const std::vector<int>& solution, int width, const std::vector<int>& board) {
    if (width % 2 == 1) return (getInversions(solution) % 2) == (getInversions(board) % 2);
    auto solutionBlankRow = getBlank(solution) / width;
    auto boardBlankRow = getBlank(board) / width;
    return getInversions(solution) % 2 != (getInversions(board) % 2 == (solutionBlankRow - boardBlankRow) % 2);
}

void initIda(const std::string& dbName, int width, int height, const std::vector<std::vector<int>>& patterns) {
    DisjointDatabase::load(patterns, dbName, width, height);
    if (width == height) {
        std::vector<int> solution = combine(patterns);
        WalkingDistance::load(solution, width, height);
    }
}

std::string getGridString(const std::vector<int>& grid) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < grid.size(); ++i) {
        oss << grid[i];
        if (i < grid.size() - 1) oss << ", ";
    }
    oss << "]";
    return oss.str();
}

template <class Node>
std::vector<int> search(const std::vector<int>& grid, int width, int height) {
    Logger::info("[IDA* Solver] Initial State: " + getGridString(grid));

    Idastar<Node> searcher;
    Node startNode(grid, width, height); 

    auto path = searcher.solve(startNode);

    std::vector<int> solution;
    std::reverse(path.begin(), path.end()); 
    
    std::ostringstream sequence; // Highly optimized sequence stream
    for (size_t i = 0; i < path.size(); ++i) {
        Direction tileMove = -path[i]; 
        solution.push_back(tileMove.getDir());
        
        if (i > 0) sequence << ", ";
        sequence << tileMove.toChar();
    }
    
    std::ostringstream seqLog;
    seqLog << "[IDA* Solver] Solution sequence: [" << sequence.str() << "]";
    Logger::info(seqLog.str());
    
    std::vector<int> finalGrid(grid.size());
    for(size_t i = 0; i < grid.size() - 1; i++) finalGrid[i] = i + 1;
    finalGrid.back() = 0;
    
    Logger::info("[IDA* Solver] Final State: " + getGridString(finalGrid));

    return solution;
}

std::vector<int> runIda(const std::vector<int>& grid, int width, int height) {
    std::vector<int> solutionState(grid.size());
    for(size_t i=0; i<grid.size()-1; i++) solutionState[i] = i+1;
    solutionState.back() = 0;

    if (!solvable(solutionState, width, grid)) return {}; 

    // NEW: Cleanly route to the proper mathematical search node
    if (width == height) {
        return search<ida::SearchNode>(grid, width, height);
    } else {
        return search<ida::SearchNodeRect>(grid, width, height);
    }
}