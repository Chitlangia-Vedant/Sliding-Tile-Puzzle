#include "../include/SearchNode.h"

#include <algorithm>
#include <functional>
#include <iomanip>
#include <numeric>
#include <unordered_map>

#include "../include/Util.h"
#include "../include/WalkingDistance.h"
namespace ida {
SearchNode::SearchNode(const std::vector<int>& g, int width, int height)
    : WIDTH(width),
      HEIGHT(height),
      deltas({-width, 1, width, -1}),
      canMoveList(calcMoveList(width, height)),
      blank(getBlank(g)),
      grid(g),
      mirrGrid(width * height),
      patterns(DisjointDatabase::calculatePatterns(g)),
      wdRowIndex(WalkingDistance::getIndex(g)),
      wdColIndex(WalkingDistance::getIndex(g, false)) {
    assertm((int)g.size() == width * height, "Wrong board dimensions");

    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        mirrGrid[i] =
            DisjointDatabase::mirrPos[grid[DisjointDatabase::mirror[i]]];
    }
    mirrPatterns = DisjointDatabase::calculatePatterns(mirrGrid);
}

int SearchNode::getHeuristic() const {
    return std::max(std::max(DisjointDatabase::getHeuristic(patterns),
                             DisjointDatabase::getHeuristic(mirrPatterns)),
                    WalkingDistance::costs[wdRowIndex] +
                        WalkingDistance::costs[wdColIndex]);
}

std::vector<Direction> SearchNode::getMoves() const {
    if (blank < WIDTH) {           
        if (blank % WIDTH == 0) return {Direction::right, Direction::down};
        if (blank % WIDTH == WIDTH - 1) return {Direction::down, Direction::left};
        return {Direction::right, Direction::down, Direction::left};
    }
    if (blank >= (WIDTH - 1) * HEIGHT) {  
        if (blank % WIDTH == 0) return {Direction::up, Direction::right};
        if (blank % WIDTH == WIDTH - 1) return {Direction::up, Direction::left};
        return {Direction::up, Direction::right, Direction::left};
    }
    if (blank % WIDTH == 0) return {Direction::up, Direction::right, Direction::down};
    if (blank % WIDTH == WIDTH - 1) return {Direction::up, Direction::down, Direction::left};

    return {Direction::up, Direction::right, Direction::down, Direction::left};
}

inline int SearchNode::getTile(int posn) const { return grid[posn]; }

inline void SearchNode::setTile(int posn, int tile) { grid[posn] = tile; }

inline int SearchNode::getMirrTile(int posn) const { return mirrGrid[posn]; }

inline void SearchNode::setMirrTile(int posn, int tile) { mirrGrid[posn] = tile; }

bool SearchNode::canMove(Direction dir) {
    return canMoveList[blank][dir.asIndex()]; 
}

inline int SearchNode::getDelta(const std::vector<int>& g, int tile,
                                int offset) const {
    // Which pattern the sliding tile is in
    const auto index = DisjointDatabase::where[tile];
    const auto delta = DisjointDatabase::tileDeltas[tile];
    return std::transform_reduce(g.cbegin() + offset + 1,
        g.cbegin() + offset + WIDTH + 1, delta, std::plus<>(),
        [&index, &tile, &delta](const auto skip) {
            if (DisjointDatabase::where[skip] != index) {
                return delta;
            } else if (skip > tile) {
                return delta + DisjointDatabase::tileDeltas[skip];
            } else {
                return 0;
            }
        });
}

// Pattern ID, pattern index
SearchNode::MoveState SearchNode::applyMove(Direction dir) {
    // Position of sliding tile (and new blank)
    const auto newBlank = blank + deltas[dir.asIndex()];
    const auto tile = getTile(newBlank);

    // Set value of slid tile
    setTile(blank, tile);

    // Update pattern
    const auto index =
        DisjointDatabase::where[tile];  // Which pattern the sliding tile is in
    const auto oldPattern = patterns[index];  // Storing for undo

    // Update mirror grid, pattern
    const auto mirrBlank = DisjointDatabase::mirror[blank];
    const auto mirrNewBlank = DisjointDatabase::mirror[newBlank];
    const auto mirrTile = getMirrTile(mirrNewBlank);
    setMirrTile(mirrBlank, mirrTile);
    const auto mirrIndex = DisjointDatabase::where[mirrTile];
    const auto oldMirrPattern = mirrPatterns[mirrIndex];

    // Walking Distance
    const auto prevRowIndex = wdRowIndex;
    const auto prevColIndex = wdColIndex;

    switch (dir.getDir()) {
        case Direction::up:
            patterns[index] += getDelta(grid, tile, newBlank);
            wdRowIndex = WalkingDistance::edgesDown[wdRowIndex][WalkingDistance::row[tile]];
            mirrPatterns[mirrIndex] += DisjointDatabase::tileDeltas[mirrTile];
            break;
        case Direction::right:
            patterns[index] -= DisjointDatabase::tileDeltas[tile];
            wdColIndex = WalkingDistance::edgesUp[wdColIndex][WalkingDistance::col[tile]];
            mirrPatterns[mirrIndex] -= getDelta(mirrGrid, mirrTile, mirrBlank);
            break;
        case Direction::down:
            patterns[index] -= getDelta(grid, tile, blank);
            wdRowIndex = WalkingDistance::edgesUp[wdRowIndex][WalkingDistance::row[tile]];
            mirrPatterns[mirrIndex] -= DisjointDatabase::tileDeltas[mirrTile];
            break;
        case Direction::left:
            patterns[index] += DisjointDatabase::tileDeltas[tile];
            wdColIndex = WalkingDistance::edgesDown[wdColIndex][WalkingDistance::col[tile]];
            mirrPatterns[mirrIndex] += getDelta(mirrGrid, mirrTile, mirrNewBlank);
            break;
        case Direction::x:  // NEW: Silences the -Wswitch compiler warning
            break;
    }

    // Update blank tile
    const auto oldBlank = blank;
    blank = newBlank;

    return {oldPattern, oldMirrPattern, prevRowIndex, prevColIndex, oldBlank};
}

void SearchNode::undoMove(const SearchNode::MoveState& prev) {
    const auto& [pattern, mirrPattern, prevRowIndex, prevColIndex, newBlank] =
        prev;

    // Value of sliding tile
    const auto tile = getTile(newBlank);

    // Set value of slid tile
    setTile(blank, tile);

    // Restore saved pattern ID
    int index = DisjointDatabase::where[tile];
    patterns[index] = pattern;

    // Update mirrored grid
    auto mirrBlank = DisjointDatabase::mirror[blank];
    auto mirrNewBlank = DisjointDatabase::mirror[newBlank];
    auto mirrTile = getMirrTile(mirrNewBlank);
    setMirrTile(mirrBlank, mirrTile);
    int mirrIndex = DisjointDatabase::where[mirrTile];
    mirrPatterns[mirrIndex] = mirrPattern;

    // Update blank tile
    blank = newBlank;

    // Update WD
    wdRowIndex = prevRowIndex;
    wdColIndex = prevColIndex;
}

std::ostream& operator<<(std::ostream& out, const SearchNode& node) {
    for (int y = 0; y < node.HEIGHT; y++) {
        for (int x = 0; x < node.WIDTH; x++) {
            int i = y * node.WIDTH + x;
            if (i == node.blank) {
                out << std::setw(3) << 0;
            } else {
                out << std::setw(3) << node.getTile(i);
            }
        }
        out << std::endl;
    }
    return out;
}
}  // namespace ida