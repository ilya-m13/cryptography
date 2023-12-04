#pragma once

#include <vector>

namespace cga::blindsign {

class Client {
  public:
    std::vector<unsigned long> vote(unsigned long d, unsigned long n);
    std::pair<unsigned long, std::vector<unsigned long>>
    bulletin(const std::vector<unsigned long> &s, unsigned long n) const;

  private:
    unsigned long _n{0};
    unsigned long _r{0};
};

} // namespace cga::blindsign
