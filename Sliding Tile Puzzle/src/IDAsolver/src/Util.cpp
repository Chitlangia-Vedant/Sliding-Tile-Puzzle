#include "../include/Util.h"

#include <algorithm>

// FIXED: Include the new unified core Direction architecture
#include "../../core/Direction.h"

namespace ida {
std::vector<int> combine(const std::vector<std::vector<int>>& grids) {
    std::vector<int> solution(grids[0].size(), 0);
    for (auto& grid : grids) {
        assertm(grid.size() == solution.size(), "Mismatching pattern sizes");
        for (std::size_t i = 0; i < grid.size(); i++) {
            if (grid[i] != 0) {
                solution[i] = grid[i];
            }
        }
    }
    return solution;
}

int getBlank(const std::vector<int>& board) {
    auto it = std::find(board.cbegin(), board.cend(), 0);
    assertm(it != board.cend(), "Blank must exist in board");
    return std::distance(board.cbegin(), it);
}

std::vector<std::array<bool, 4>> calcMoveList(int width, int height) {
    // [index][direction]
    std::vector<std::array<bool, 4>> moves(width * height,
                                           std::array<bool, 4>{});

    // Blank position
    for (int i = 0; i < width * height; i++) {
        // FIXED: Create a temporary Direction object and cleanly map it to the array index
        moves[i][Direction{Direction::up}.asIndex()] = (i / width) > 0;
        moves[i][Direction{Direction::right}.asIndex()] = (i % width) < width - 1;
        moves[i][Direction{Direction::down}.asIndex()] = (i / width) < height - 1;
        moves[i][Direction{Direction::left}.asIndex()] = (i % width) > 0;
    }

    return moves;
}
}