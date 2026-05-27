#include "IdaBridge.h"

// Now it is safe to include the IDA headers because they are in the 'ida' namespace
#include "../IDAsolver/include/Board.h"
#include "../IDAsolver/include/BoardRect.h"
#include "../IDAsolver/include/DisjointDatabase.h"
#include "../IDAsolver/include/WalkingDistance.h"
#include "../IDAsolver/include/Idastar.h"
#include "../IDAsolver/include/Util.h"
#include <algorithm>
#include <iostream>

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

template <class B>
std::vector<int> search(const std::vector<int>& grid, int width, int height) {
    Idastar<B> searcher;
    B board(grid, width, height);

    auto path = searcher.solve(board);

    std::vector<int> solution;
    std::reverse(path.begin(), path.end()); // Required by the original solver logic
    for (auto move : path) {
        solution.push_back(static_cast<int>(move));
    }
    return solution;
}

std::vector<int> runIda(const std::vector<int>& grid, int width, int height) {
    // Generate a temporary solution array (1 to N, with 0 at the end) for the solvable check
    std::vector<int> solutionState(grid.size());
    for(size_t i=0; i<grid.size()-1; i++) solutionState[i] = i+1;
    solutionState.back() = 0;

    if (!solvable(solutionState, width, grid)) {
        return {}; // Unsolvable, return empty
    }

    if (width == height) {
        return search<ida::Board>(grid, width, height);
    } else {
        return search<ida::BoardRect>(grid, width, height);
    }
}