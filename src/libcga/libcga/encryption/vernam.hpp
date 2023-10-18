#pragma once

#include <filesystem>

namespace cga::encryption {

class Vernam {
  public:
    static std::pair<unsigned char, unsigned char> encrypt(unsigned char byte);
    static unsigned char decrypt(unsigned char byte, unsigned char key);

    static bool encrypt_file(const std::filesystem::path &in);
};

} // namespace cga::encryption
