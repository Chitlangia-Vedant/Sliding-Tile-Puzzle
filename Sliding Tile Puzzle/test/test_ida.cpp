#include <iostream>
#include <vector>
#include <string>
#include "../src/wasm/IdaBridge.h"

int main() {
    // 1. Define the 4x4 555-reg patterns exactly as we did in JS
    std::vector<std::vector<int>> patterns = {
        {1, 2, 3, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 4, 0, 0, 7, 8, 0, 0, 11, 12, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 9, 10, 0, 0, 13, 14, 15, 0}
    };

    std::cout << "Initializing IDA* solver with 555-reg database...\n";
    initIda("555-reg", 4, 4, patterns);

    // 2. Define a scrambled 4x4 grid. 
    // This is a simple 2-move scramble from solved state:
    // 1 2 3 4 | 5 6 7 8 | 9 10 11 12 | 13 14 0 15
    std::vector<int> grid = {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 11, 12,
       13,  0, 14, 15
    };

    std::cout << "Running IDA* search on grid...\n";
    std::vector<int> solution = runIda(grid, 4, 4);

    if (solution.empty()) {
        std::cout << "No solution found (or already solved)!\n";
    } else {
        std::cout << "Optimal Solution found in " << solution.size() << " moves:\n";
        for (int dir : solution) {
            std::cout << dir << " ";
        }
        std::cout << "\n";
    }

    return 0;
}