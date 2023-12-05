#pragma once

#include <vector>

namespace cga::zkprot {

using AdjMatrix = std::vector<std::vector<bool>>;
using HamilPath = std::vector<unsigned int>;

using Matrix = std::vector<std::vector<unsigned long>>;

struct EncryptVer {
    EncryptVer(unsigned int v1, unsigned int v2, unsigned long value)
        : _value(value) {
        _vers.first = v1;
        _vers.second = v2;
    }

    std::pair<unsigned int, unsigned int> _vers;
    unsigned long _value;
};

} // namespace cga::zkprot