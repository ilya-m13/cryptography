#pragma once

#include <libcga/graphcol/types.hpp>

#include <istream>

namespace cga::graphcol {

std::pair<ColorVec, EdgeVec> parse(std::istream &is);

} // namespace cga::graphcol
