#pragma once

#include <libcga/zkprot/types.hpp>

#include <istream>

namespace cga::zkprot {

std::pair<AdjMatrix, HamilPath> parse(std::istream &is);

} // namespace cga::zkprot
