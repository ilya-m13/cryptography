#include <libcga/encryption/vernam.hpp>

#include <fstream>
#include <random>

static std::filesystem::path append_filename(
    const std::filesystem::path &in, const std::filesystem::path &name) {
    auto out = in;
    out.replace_filename(in.stem() += name);
    out.replace_extension(in.extension());
    return out;
}

namespace cga::encryption {

// first - e
// second - key
std::pair<unsigned char, unsigned char> Vernam::encrypt(unsigned char byte) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned char> dist;

    unsigned char key = dist(gen);
    unsigned char e = byte ^ key;
    return {e, key};
}

unsigned char Vernam::decrypt(unsigned char byte, unsigned char key) {
    return byte ^ key;
}

bool Vernam::encrypt_file(const std::filesystem::path &in) {
    std::ifstream is(in, std::ios_base::binary);
    if (!is.is_open()) {
        return false;
    }

    std::array<std::ofstream, 2> os;
    os[0].open(append_filename(in, "_VernamEncrypt"), std::ios_base::binary);
    os[1].open(append_filename(in, "_VernamDecrypt"), std::ios_base::binary);

    unsigned char byte = 0;
    while (is >> byte) {
        const auto pair = encrypt(byte);
        os[0] << pair.first;

        byte = decrypt(pair.first, pair.second);
        os[1] << byte;
    }

    return true;
}

} // namespace cga::encryption
