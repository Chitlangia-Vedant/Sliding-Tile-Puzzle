#include "../include/SearchNodeRect.h"

#include <algorithm>
#include <functional>
#include <iomanip>
#include <numeric>
#include <unordered_map>

#include "../include/Util.h"
#include "../include/WalkingDistance.h"

// WIDTH < HEIGHT
namespace ida {
SearchNodeRect::SearchNodeRect(const std::vector<int>& g, int width, int height)
    : WIDTH(width),
      HEIGHT(height),
      deltas({-width, 1, width, -1}),
      canMoveList(calcMoveList(width, height)),
      blank(getBlank(g)),
      grid(g),
      patterns(DisjointDatabase::calculatePatterns(g)) {}

int SearchNodeRect::getHeuristic() const {
    return DisjointDatabase::getHeuristic(patterns);
}

std::vector<Direction> SearchNodeRect::getMoves() const {
    if (blank < WIDTH) {           // top
        if (blank % WIDTH == 0) {  // left
            return {Direction::right, Direction::down};
        }
        if (blank % WIDTH == WIDTH - 1) {  // right
            return {Direction::down, Direction::left};
        }
        return {Direction::right, Direction::down, Direction::left};
    }
    if (blank >= (WIDTH - 1) * HEIGHT) {  // bottom
        if (blank % WIDTH == 0) {         // left
            return {Direction::up, Direction::right};
        }
        if (blank % WIDTH == WIDTH - 1) {  // right
            return {Direction::up, Direction::left};
        }
        return {Direction::up, Direction::right, Direction::left};
    }
    if (blank % WIDTH == 0) {  // left
        return {Direction::up, Direction::right, Direction::down};
    }
    if (blank % WIDTH == WIDTH - 1) {  // right
        return {Direction::up, Direction::down, Direction::left};
    }

    return {Direction::up, Direction::right, Direction::down, Direction::left};
}

inline int SearchNodeRect::getTile(int posn) const { return grid[posn]; }

inline void SearchNodeRect::setTile(int posn, int tile) { grid[posn] = tile; }

bool SearchNodeRect::canMove(Direction dir) {
    // FIXED: Use asIndex() instead of static_cast
    return canMoveList[blank][dir.asIndex()];
}

inline int SearchNodeRect::getDelta(const std::vector<int>& g, int tile,
                                    int offset) const {
    // Which pattern the sliding tile is in
    const auto index = DisjointDatabase::where[tile];
    const auto delta = DisjointDatabase::tileDeltas[tile];
    return std::transform_reduce(
        g.cbegin() + offset + 1, g.cbegin() + offset + WIDTH + 1, delta,
        std::plus<>(), [&index, &tile, &delta](const auto skip) {
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
SearchNodeRect::MoveState SearchNodeRect::applyMove(Direction dir) {
    // FIXED: Use asIndex()
    // Position of sliding tile (and new blank)
    const auto newBlank = blank + deltas[dir.asIndex()];
    // Value of sliding tile
    const auto tile = getTile(newBlank);

    // Set value of slid tile
    setTile(blank, tile);

    // Update pattern
    const auto index =
        DisjointDatabase::where[tile];  // Which pattern the sliding tile is in
    const auto oldPattern = patterns[index];  // Storing for undo

    // FIXED: Check core direction type natively
    switch (dir.getDir()) {
        case Direction::up:
            patterns[index] += getDelta(grid, tile, newBlank);
            break;
        case Direction::right:
            patterns[index] -= DisjointDatabase::tileDeltas[tile];
            break;
        case Direction::down:
            patterns[index] -= getDelta(grid, tile, blank);
            break;
        case Direction::left:
            patterns[index] += DisjointDatabase::tileDeltas[tile];
            break;
        case Direction::x:  // NEW: Silences the -Wswitch compiler warning
            break;
    }

    // Update blank tile
    const auto oldBlank = blank;
    blank = newBlank;

    return {oldPattern, oldBlank};
}

void SearchNodeRect::undoMove(const SearchNodeRect::MoveState& prev) {
    const auto& [pattern, newBlank] = prev;

    // Value of sliding tile
    const auto tile = getTile(newBlank);

    // Set value of slid tile
    setTile(blank, tile);

    // Restore saved pattern ID
    int index = DisjointDatabase::where[tile];
    patterns[index] = pattern;

    // Update blank tile
    blank = newBlank;
}

std::ostream& operator<<(std::ostream& out, const SearchNodeRect& node) {
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