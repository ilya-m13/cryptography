#pragma once

#include <filesystem>

namespace cga::encryption {

class ElGamal {
  public:
    ElGamal();
    ElGamal(unsigned long p, unsigned long g);

    std::pair<unsigned long, unsigned long>
    encrypt(unsigned long m, unsigned long d) const;
    void encrypt(std::istream &is, std::ostream &os, unsigned long d) const;

    unsigned long decrypt(std::pair<unsigned long, unsigned long> data) const;
    void decrypt(std::istream &is, std::ostream &os) const;

    static bool encrypt_file(const std::filesystem::path &in);

    unsigned long p() const;
    unsigned long g() const;
    unsigned long d() const;

  private:
    unsigned long p_;
    unsigned long g_;
    unsigned long c_;
    unsigned long d_;
};

} // namespace cga::encryption