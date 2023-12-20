#pragma once

#include <libcga/graphcol/types.hpp>

namespace cga::graphcol {

class User {
  public:
    struct VerData {
        std::vector<unsigned long> n_;
        std::vector<unsigned long> d_;
        std::vector<unsigned long> z_;
    };

    explicit User(EdgeVec edgevec);
    User(EdgeVec edgevec, ColorVec colorvec);

    VerData request();
    std::pair<unsigned long, unsigned long> select_edge(const Edge &edge) const;
    Edge select_edge();

    bool verify(
        const VerData &verdata,
        std::pair<unsigned long, unsigned long> en_edge) const;

  private:
    ColorVec _color_pmt = {R, B, Y};

    EdgeVec _edgevec;
    ColorVec _colorvec;

    Edge selected_edge;

    std::vector<unsigned long> _c;
};

} // namespace cga::graphcol
