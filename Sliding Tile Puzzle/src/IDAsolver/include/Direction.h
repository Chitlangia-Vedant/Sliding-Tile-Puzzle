#ifndef DIRECTION_H
#define DIRECTION_H

#include <ostream>
namespace ida {

enum class Direction { U, R, D, L };

std::ostream& operator<<(std::ostream& out, const Direction& dir);
}  // namespace ida
#endif  // DIRECTION_H

