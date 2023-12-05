#include <libcga/zkprot/parser.hpp>

namespace cga::zkprot {

std::pair<AdjMatrix, HamilPath> parse(std::istream &is) {
    unsigned int n = 0;
    unsigned int m = 0;
    is >> n >> m;
    is.get();

    HamilPath hamilpath;
    AdjMatrix mtx(n);
    for (auto &vec : mtx) {
        vec.resize(n);
    }

    while (is.peek() != '\n') {
        unsigned int v = 0;
        is >> v;
        hamilpath.push_back(v);
    }

    unsigned int v1 = 0;
    unsigned int v2 = 0;
    while (is >> v1 >> v2) {
        mtx[v1][v2] = true;
        mtx[v2][v1] = true;
    }

    return {mtx, hamilpath};
}

} // namespace cga::zkprot