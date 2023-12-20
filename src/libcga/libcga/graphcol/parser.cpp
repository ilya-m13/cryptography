#include <libcga/graphcol/parser.hpp>

namespace cga::graphcol {

std::pair<ColorVec, EdgeVec> parse(std::istream &is) {
    unsigned int n = 0;
    unsigned int m = 0;
    is >> n >> m;
    is.get();

    ColorVec colorvec;
    EdgeVec edgevec;

    while (is.peek() != '\n') {
        char color = 0;
        is >> color;

        switch (color) {
        case 'Y':
            colorvec.push_back(Y);
            break;
        case 'B':
            colorvec.push_back(B);
            break;
        case 'R':
            colorvec.push_back(R);
            break;
        default:
            break;
        }
    }

    Ver v1 = 0;
    Ver v2 = 0;
    while (is >> v1 >> v2) {
        edgevec.push_back({v1, v2});
    }

    return {colorvec, edgevec};
}

} // namespace cga::graphcol