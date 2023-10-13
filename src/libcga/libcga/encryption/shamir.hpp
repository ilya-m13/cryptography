#pragma once

#include <filesystem>

namespace cga::encryption {

class Shamir {
  public:
    Shamir();
    explicit Shamir(unsigned long P);
    unsigned long encrypt(unsigned long value) const;
    unsigned long decrypt(unsigned long value) const;
    unsigned long P() const;

    static bool encrypt_file(const std::filesystem::path &in);

  private:
    void generate_P();
    void generate_CD(unsigned long P);

    unsigned long P_;
    unsigned long C_;
    unsigned long D_;
};

} // namespace cga::encryption
