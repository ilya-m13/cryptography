#pragma once

#include <vector>

namespace cga::blindsign {

class Server {
  public:
    Server();

    std::vector<unsigned long> sign(const std::vector<unsigned long> &h) const;
    bool verify(unsigned long n, const std::vector<unsigned long> &s) const;

    void reset();

    unsigned long n() const;
    unsigned long d() const;

  private:
    unsigned long _p;
    unsigned long _q;
    unsigned long _n;
    unsigned long _c;
    unsigned long _d;
};

} // namespace cga::blindsign
