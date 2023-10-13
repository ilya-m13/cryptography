#include <libcga/encryption/shamir.hpp>

#include <libcga/base_functions/base_functions.hpp>

#include <fstream>
#include <limits>
#include <random>

const unsigned long k = 100;

static std::filesystem::path append_filename(
    const std::filesystem::path &in, const std::filesystem::path &name) {
    auto out = in;
    out.replace_filename(in.stem() += name);
    out.replace_extension(in.extension());
    return out;
}

namespace cga::encryption {

// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init,hicpp-member-init)

Shamir::Shamir() {
    generate_P();
    generate_CD(P_);
}

Shamir::Shamir(unsigned long P) : P_(P) {
    generate_CD(P);
}

// NOLINTEND(cppcoreguidelines-pro-type-member-init,hicpp-member-init)

unsigned long Shamir::encrypt(unsigned long value) const {
    return cga::base_functions::modular_exponentiation(value, C_, P_);
}

unsigned long Shamir::decrypt(unsigned long value) const {
    return cga::base_functions::modular_exponentiation(value, D_, P_);
}

void Shamir::generate_P() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long> dist(
        std::numeric_limits<unsigned char>::max() + 1,
        std::numeric_limits<int>::max());

    do {
        P_ = dist(gen);
    } while (P_ % 2 == 0 || !cga::base_functions::miller_rabin_test(P_, k));
}

void Shamir::generate_CD(unsigned long P) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long> dist(
        2, std::numeric_limits<int>::max());

    do {
        unsigned long value = dist(gen);
        C_ = value % 2 == 0 ? value + 1 : value;
    } while (cga::base_functions::extended_euclidean(
                 static_cast<int>(std::max({C_, P - 1})),
                 static_cast<int>(std::min({C_, P - 1})))[0] != 1);

    const int result = C_ >= P - 1
        ? cga::base_functions::extended_euclidean(
              static_cast<int>(C_), static_cast<int>(P - 1))[1]
        : cga::base_functions::extended_euclidean(
              static_cast<int>(P - 1), static_cast<int>(C_))[2];
    D_ = result < 0 ? result + P - 1 : result;
}

unsigned long Shamir::P() const {
    return P_;
}

bool Shamir::encrypt_file(const std::filesystem::path &in) {
    std::ifstream is(in, std::ios_base::binary);
    if (!is.is_open()) {
        return false;
    }

    Shamir alice;
    Shamir bob(alice.P());

    std::array<std::ofstream, 4> os;
    os[0].open(append_filename(in, "_Aen"), std::ios_base::binary);
    os[1].open(append_filename(in, "_Ben"), std::ios_base::binary);
    os[2].open(append_filename(in, "_Ade"), std::ios_base::binary);
    os[3].open(append_filename(in, "_Bde"), std::ios_base::binary);

    unsigned long value;
    while (is.read(reinterpret_cast<char *>(&value), sizeof(char))) {
        value = alice.encrypt(value);
        os[0].write(reinterpret_cast<char *>(&value), sizeof(unsigned long));

        value = bob.encrypt(value);
        os[1].write(reinterpret_cast<char *>(&value), sizeof(unsigned long));

        value = alice.decrypt(value);
        os[2].write(reinterpret_cast<char *>(&value), sizeof(unsigned long));

        value = bob.decrypt(value);
        os[3].write(reinterpret_cast<char *>(&value), sizeof(char));
    }

    return true;
}

} // namespace cga::encryption
