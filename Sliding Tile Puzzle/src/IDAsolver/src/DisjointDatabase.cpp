#include "../include/DisjointDatabase.h"

#include <limits>
#include <numeric>
#include <fstream>
#include <unordered_map>

#include "../include/Pattern.h"
#include "../include/Util.h"
namespace ida {
using Grid = DisjointDatabase::Grid;
using Hash = DisjointDatabase::Hash;
using Cost = uint_fast8_t;

using DisjointDatabase::height;
using DisjointDatabase::tileDeltas;
using DisjointDatabase::where;
using DisjointDatabase::width;

constexpr Cost INF = std::numeric_limits<Cost>::max();  // 255

int DisjointDatabase::width;
int DisjointDatabase::height;

std::vector<std::vector<int>> patternTiles;
std::vector<std::vector<Cost>> costs;
Grid DisjointDatabase::where;
Grid DisjointDatabase::tileDeltas;
Grid DisjointDatabase::mirrPos;
Grid DisjointDatabase::mirror;

std::vector<Cost> loadPattern(const std::string& filename, int size) {
    std::vector<Cost> patternCosts(size);
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open pattern database file: " + filename);
    }
    
    file.read(reinterpret_cast<char*>(patternCosts.data()), size * sizeof(Cost));
    return patternCosts;
}

void calculatePatternTiles() {
    patternTiles.resize(costs.size());

    for (int i = 1; i < width * height; i++) {  // Ignore blank tile (0)
        patternTiles[where[i]].push_back(i);
    }
}

void calculateDeltas() {
    tileDeltas = std::vector<int>(width * height, 1);

    for (auto& tiles : patternTiles) {
        for (int j = (int)tiles.size() - 2; j >= 0; j--) {
            tileDeltas[tiles[j]] =
                tileDeltas[tiles[j + 1]] * (width * height - 1 - j);
        }
    }
}

void DisjointDatabase::load(const std::vector<Grid>& patterns,
                            const std::string& name, int w, int h) {
    width = w;
    height = h;
    auto length = w * h;

    costs.clear();
    patternTiles.clear();
    where.resize(length, -1);
    mirrPos.resize(length, 0);
    mirror.resize(length);
    // TODO: test with blank not in top-left or bottom-right
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            mirror[y * width + x] = x * width + y;
        }
    }

    for (std::size_t i = 0; i < patterns.size(); i++) {
        DEBUG("Pattern #" << i << ':');

        int size = 1;      // # of entries in database
        int numTiles = 0;  // # of tiles in partial pattern

        for (int j = 0; j < length; j++) {
            int tile = patterns[i][j];
            if (tile > 0) {
                size *= length - numTiles;
                numTiles++;

                where[tile] = i;
            }
        }

        costs.push_back(loadPattern(
            "databases/" + name + "-" + std::to_string(i) + ".dat", size));
    }

    // All partial grids, layered to one
    // This represents the solved grid
    auto combined = combine(patterns);
    for (int i = 0; i < length; i++) {
        if (combined[i] > 0) {
            mirrPos[combined[i]] = combined[mirror[i]];
        }
    }

    calculatePatternTiles();
    calculateDeltas();
}

std::vector<Hash> DisjointDatabase::calculatePatterns(const Grid& grid) {
    auto length = width * height;
    std::vector<Hash> pat(costs.size(), 0);

    for (int i = 0; i < (int)costs.size(); i++) {
        // Calculate pattern
        std::vector<int> startPos(length, 0);
        std::unordered_map<int, int> before;
        for (int j = 0; j < length; j++) {
            if (where[grid[j]] == i) {
                // New tile found
                int beforeCount = 0;

                // Count number of preceding pattern tiles that's smaller
                for (auto& it : before) {
                    if (it.first < grid[j]) {
                        beforeCount++;
                    }
                }

                before[grid[j]] = beforeCount;
                startPos[grid[j]] = j;
            }
        }

        int j = length;
        for (auto tile : patternTiles[i]) {
            pat[i] *= j--;
            pat[i] += startPos[tile] - before[tile];
        }
    }

    return pat;
}

int DisjointDatabase::getHeuristic(const std::vector<Hash>& patterns) {
    return std::transform_reduce(
        costs.cbegin(), costs.cend(), patterns.cbegin(), 0, std::plus<>(),
        [](const auto& cost, const auto& pattern) { return cost[pattern]; });
}
}  // namespace ida