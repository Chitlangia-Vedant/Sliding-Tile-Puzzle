#include "../include/Idastar.h"

#include "../include/SearchNode.h"
#include "../include/SearchNodeRect.h"
#include "../include/Util.h"
#include <limits> // NEW: Include limits for true infinity

constexpr int INF = std::numeric_limits<int>::max();

namespace ida {
template <class B>
Idastar<B>::Idastar() : path({}), minCost(INF), limit(0), nodes(0) {}

template <class B>
std::vector<Direction> Idastar<B>::solve(const B& start) {
    path.clear();
    nodes = 1;
    limit = start.getHeuristic();

    if (limit == 0) {
        DEBUG("Already solved");
        return path;
    }

    DEBUG("Limit, Nodes:");

    // Starting moves (for prevMove)
    auto startMoves = start.getMoves();

    while (path.empty()) {
        minCost = INF;
        DEBUG("  " << limit << ", " << nodes);
        for (auto startDir : startMoves) {
            auto copy = start;
            copy.applyMove(startDir);

            if (dfs(copy, 1, -startDir)) {
                path.push_back(startDir);
                DEBUG("  Final Nodes Expanded: " << nodes);
                return path;
            }
        }
        limit = minCost;
    }

    return path;
}

template <class B>
bool Idastar<B>::dfs(B& node, int g, Direction prevMove) {
    auto h = node.getHeuristic();
    auto f = g + h;

    if (h == 0) [[unlikely]] return true;
    else if (f > limit) {
        if (f < minCost) minCost = f;
        return false;
    }

    nodes += 1;

    // NEW: Cleanly iterate over the unified directions array
    for (const auto& dir : Direction::ALL_DIRS) {
        if (prevMove != dir && node.canMove(dir)) {
            auto prev = node.applyMove(dir);

            // NEW: Pass -dir to recursively invert
            if (dfs(node, g + 1, -dir)) {
                path.push_back(dir);
                return true;
            }

            node.undoMove(prev);
        }
    }
    return false;
}

template class Idastar<SearchNode>;
template class Idastar<SearchNodeRect>;
}  // namespace ida