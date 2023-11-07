#pragma once

#include <filesystem>
#include <vector>

namespace cga::signature {

class RSA {
  public:
    RSA();

    std::vector<unsigned long> sign(const char *m, std::size_t size) const;
    bool verify(
        const std::vector<unsigned long> &s,
        const char *m,
        std::size_t size,
        unsigned long d,
        unsigned long N) const;

    unsigned long N() const;
    unsigned long d() const;

    static bool sign_file(const std::filesystem::path &in);

  private:
    unsigned long N_;
    unsigned long c_;
    unsigned long d_;
};

} // namespace cga::signature
