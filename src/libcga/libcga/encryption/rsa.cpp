#include <libcga/encryption/rsa.hpp>

#include <libcga/base_functions/base_functions.hpp>

#include <fstream>
#include <limits>
#include <random>

const unsigned long max_gen_value = 10000;
const unsigned long k = 100;

static std::filesystem::path append_filename(
    const std::filesystem::path &in, const std::filesystem::path &name) {
    auto out = in;
    out.replace_filename(in.stem() += name);
    out.replace_extension(in.extension());
    return out;
}

namespace cga::encryption {

RSA::RSA() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long> dist(2, max_gen_value);

    unsigned long P, Q;
    do {
        P = dist(gen);
    } while (!cga::base_functions::miller_rabin_test(P, k));
    do {
        Q = dist(gen);
    } while (!cga::base_functions::miller_rabin_test(Q, k));
    N_ = P * Q;

    unsigned long phi = (P - 1) * (Q - 1);
    do {
        d_ = dist(gen) % (phi - 2) + 2;
    } while (cga::base_functions::extended_euclidean(
                 static_cast<long>(phi), static_cast<long>(d_))[0] != 1);
    const long y = cga::base_functions::extended_euclidean(
        static_cast<long>(phi), static_cast<long>(d_))[2];
    c_ = y < 0 ? y + phi : y;
}

// m < N
unsigned long
RSA::encrypt(unsigned long m, unsigned long d, unsigned long N) const {
    return cga::base_functions::modular_exponentiation(m, d, N);
}

unsigned long RSA::decrypt(unsigned long e) const {
    return cga::base_functions::modular_exponentiation(e, c_, N_);
}

unsigned long RSA::N() const {
    return N_;
}

unsigned long RSA::d() const {
    return d_;
}

bool RSA::encrypt_file(const std::filesystem::path &in) {
    std::ifstream is(in, std::ios_base::binary);
    if (!is.is_open()) {
        return false;
    }

    RSA alice;
    RSA bob;

    std::array<std::ofstream, 2> os;
    os[0].open(append_filename(in, "_RSAencrypt"), std::ios_base::binary);
    os[1].open(append_filename(in, "_RSAdecrypt"), std::ios_base::binary);

    unsigned long m = 0;
    while (is.read(reinterpret_cast<char *>(&m), sizeof(char))) {
        m = alice.encrypt(m, bob.d(), bob.N());
        os[0].write(reinterpret_cast<char *>(&m), sizeof(unsigned long));

        m = bob.decrypt(m);
        os[1].write(reinterpret_cast<char *>(&m), sizeof(char));
    }

    return true;
}

} // namespace cga::encryption
