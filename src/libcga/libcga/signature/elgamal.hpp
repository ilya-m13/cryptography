#pragma once

#include <filesystem>
#include <vector>

namespace cga::signature {

class ElGamal {
  public:
    ElGamal();
    ElGamal(unsigned long p, unsigned long g);

    std::pair<unsigned long, std::vector<unsigned long>>
    sign(const char *m, std::size_t size) const;
    bool verify(
        const std::pair<unsigned long, std::vector<unsigned long>> &pair,
        const char *m,
        std::size_t size,
        unsigned long d) const;

    static bool sign_file(const std::filesystem::path &in);

    unsigned long p() const;
    unsigned long g() const;
    unsigned long d() const;

  private:
    unsigned long p_;
    unsigned long g_;
    unsigned long c_;
    unsigned long d_;
};

} // namespace cga::signature