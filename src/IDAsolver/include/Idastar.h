
#ifndef IDASTAR_H
#define IDASTAR_H

#include <vector>

#include "../../core/Direction.h"

namespace ida {

template <class B>
class Idastar {
private:
    std::vector<Direction> path;
    int minCost;
    int limit;
    long long nodes;

public:
    Idastar();

    std::vector<Direction> solve(const B& start);
    bool dfs(B& root, int g, Direction prevMove);
};

}  // namespace ida

#endif
