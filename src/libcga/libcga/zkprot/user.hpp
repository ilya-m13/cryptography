#pragma once

#include <libcga/zkprot/types.hpp>

#include <vector>

namespace cga::zkprot {

class User {
  public:
    struct PathVerData {
        Matrix _f;
        std::vector<EncryptVer> _en_vers;
    };
    struct IsomorphicVerData {
        Matrix _f;
        Matrix _en_h;
        std::vector<unsigned int> _pmt;
    };

    explicit User(AdjMatrix g);
    User(AdjMatrix g, HamilPath hpath);

    PathVerData what_hpath();
    IsomorphicVerData is_isomorphic();

    bool
    verify(const PathVerData &pathdata, unsigned long d, unsigned long n) const;
    bool verify(
        const IsomorphicVerData &isdata,
        unsigned long d,
        unsigned long n) const;

    unsigned long d() const;
    unsigned long n() const;

  private:
    Matrix encrypt_mtx();
    void set_initial_values();

    AdjMatrix _g;
    HamilPath _hpath;
    std::vector<unsigned int> _pmt;

    unsigned long _p;
    unsigned long _q;
    unsigned long _n;
    unsigned long _c;
    unsigned long _d;
};

} // namespace cga::zkprot
