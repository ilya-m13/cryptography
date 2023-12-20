#pragma once

#include <vector>

namespace cga::graphcol {

enum Color { R = 0, B = 1, Y = 2 };

using Ver = unsigned int;

struct Edge {
    Ver ver1_ = 0;
    Ver ver2_ = 0;
};

using EdgeVec = std::vector<Edge>;

using ColorVec = std::vector<unsigned short>;

} // namespace cga::graphcol