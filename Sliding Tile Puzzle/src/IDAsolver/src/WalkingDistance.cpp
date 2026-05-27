#include "../include/WalkingDistance.h"

#include <algorithm>
#include <limits>
#include <fstream>
#include "../include/Util.h"
namespace ida {
using Board = std::vector<int>;
using Table = std::vector<std::vector<int>>;
using Hash = std::string;
using Cost = WalkingDistance::Cost;
using Index = WalkingDistance::Index;

using WalkingDistance::col;
using WalkingDistance::costs;
using WalkingDistance::edgesDown;
using WalkingDistance::edgesUp;
using WalkingDistance::height;
using WalkingDistance::row;
using WalkingDistance::width;

std::vector<Hash> tables;
std::vector<Cost> WalkingDistance::costs;
std::vector<std::vector<Index>> WalkingDistance::edgesUp;
std::vector<std::vector<Index>> WalkingDistance::edgesDown;

std::vector<int> WalkingDistance::row;  // Row #
std::vector<int> WalkingDistance::col;  // Column #

int WalkingDistance::width;
int WalkingDistance::height;

Table calculateTable(const Board& grid, bool alongRow = true) {
    Table table(height, std::vector<int>(width, 0));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int tile = grid[y * width + x];
            if (tile > 0) {
                if (alongRow) {
                    table[y][row[tile]]++;
                } else {
                    table[x][col[tile]]++;
                }
            }
        }
    }

    return table;
}

Hash calculateHash(const Table& table) {
    // Compress WD tables
    Hash hash = "";

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            hash += (char)table[y][x];
        }
    }

    return hash;
}

void WalkingDistance::load(const std::vector<int>& goal, int w, int h) {
    assertm(w == h, "Walking Distance requires square boards");

    width = w;
    height = h;
    auto length = w * h;

    row.resize(length);
    col.resize(length);

    // Calculate row / column indices
    for (int i = 0; i < length; i++) {
        row[goal[i]] = i / width;
        col[goal[i]] = i % width;
    }

    std::string filename = "databases/" + std::to_string(w) + "-wd.dat";
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open WD database file: " + filename);
    }

    // Read database from file
    DEBUG("Loading WD database from " << filename);

    // Store size in .dat file
    int size = 0;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));

    tables.resize(size);
    costs.resize(size);
    edgesUp.resize(size);
    edgesDown.resize(size);

    for (auto& table : tables) {
        uint64_t tableSize;
        file.read(reinterpret_cast<char*>(&tableSize), sizeof(tableSize));
        table.resize(tableSize);
        file.read(table.data(), tableSize);
    }
    for (auto& cost : costs) {
        file.read(reinterpret_cast<char*>(&cost), sizeof(cost));
    }
    for (auto& edge : edgesUp) {
        edge.resize(width);
        for (auto& col : edge) {
            file.read(reinterpret_cast<char*>(&col), sizeof(col));
        }
    }
    for (auto& edge : edgesDown) {
        edge.resize(width);
        for (auto& col : edge) {
            file.read(reinterpret_cast<char*>(&col), sizeof(col));
        }
    }
    DEBUG("Done loading WD");
}

int WalkingDistance::getIndex(const Board& grid, bool alongRow) {
    auto hash = calculateHash(calculateTable(grid, alongRow));

    // Convert to index
    auto it = std::find(tables.cbegin(), tables.cend(), hash);
    assertm(it != tables.end(), "Missing walking distance table");
    auto index = std::distance(tables.cbegin(), it);

    return index;
}
}