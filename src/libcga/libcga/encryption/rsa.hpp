#pragma once

#include <filesystem>

namespace cga::encryption {

class RSA {
  public:
    RSA();

    unsigned long
    encrypt(unsigned long m, unsigned long d, unsigned long N) const;
    unsigned long decrypt(unsigned long e) const;

    unsigned long N() const;
    unsigned long d() const;

    static bool encrypt_file(const std::filesystem::path &in);

  private:
    unsigned long N_;
    unsigned long c_;
    unsigned long d_;
};

} // namespace cga::encryption