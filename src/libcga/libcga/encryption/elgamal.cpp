#include <libcga/encryption/elgamal.hpp>

#include <libcga/base_functions/base_functions.hpp>

#include <fstream>
#include <limits>
#include <random>

const unsigned long k = 100;
const unsigned long max_gen_value = 1000000000;

static std::filesystem::path append_filename(
    const std::filesystem::path &in, const std::filesystem::path &name) {
    auto out = in;
    out.replace_filename(in.stem() += name);
    out.replace_extension(in.extension());
    return out;
}

namespace cga::encryption {

ElGamal::ElGamal() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long> dist(
        std::numeric_limits<unsigned char>::max() + 1, max_gen_value);

    unsigned long Q = 0;
    do {
        p_ = dist(gen);
        Q = (p_ - 1) / 2;
    } while (p_ % 2 == 0 || p_ < 5 ||
             !cga::base_functions::miller_rabin_test(p_, k) ||
             !cga::base_functions::miller_rabin_test(Q, k));

    do {
        g_ = dist(gen) % p_;
    } while (g_ < 2 &&
             cga::base_functions::modular_exponentiation(g_, Q, p_) == 1);

    std::uniform_int_distribution<unsigned long> dist_c(2, p_ - 2);
    c_ = dist_c(gen);
    d_ = cga::base_functions::modular_exponentiation(g_, c_, p_);
}

ElGamal::ElGamal(unsigned long p, unsigned long g) : p_(p), g_(g) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long> dist(2, p_ - 2);

    c_ = dist(gen);
    d_ = cga::base_functions::modular_exponentiation(g_, c_, p_);
}

// m < p
// pair.first - r
// pair.second - e
std::pair<unsigned long, unsigned long>
ElGamal::encrypt(unsigned long m, unsigned long d) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long> dist(1, p_ - 2);

    unsigned long k = dist(gen);

    std::pair<unsigned long, unsigned long> pair;
    pair.first = cga::base_functions::modular_exponentiation(g_, k, p_);
    pair.second =
        (m * cga::base_functions::modular_exponentiation(d, k, p_)) % p_;

    return pair;
}

void ElGamal::encrypt(
    std::istream &is, std::ostream &os, unsigned long d) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long> dist(1, p_ - 2);

    const unsigned long k = dist(gen);
    const unsigned long r =
        cga::base_functions::modular_exponentiation(g_, k, p_);

    unsigned long m = 0;
    os.write(reinterpret_cast<const char *>(&r), sizeof(unsigned long));
    while (is.read(reinterpret_cast<char *>(&m), sizeof(char))) {
        const unsigned long e =
            (m * cga::base_functions::modular_exponentiation(d, k, p_)) % p_;
        os.write(reinterpret_cast<const char *>(&e), sizeof(unsigned long));
    }
}

unsigned long
ElGamal::decrypt(std::pair<unsigned long, unsigned long> data) const {
    return (data.second *
            cga::base_functions::modular_exponentiation(
                data.first, p_ - 1 - c_, p_)) %
        p_;
}

void ElGamal::decrypt(std::istream &is, std::ostream &os) const {
    unsigned long r = 0;
    is.read(reinterpret_cast<char *>(&r), sizeof(unsigned long));

    unsigned long e = 0;
    while (is.read(reinterpret_cast<char *>(&e), sizeof(unsigned long))) {
        const unsigned long result =
            (e *
             cga::base_functions::modular_exponentiation(r, p_ - 1 - c_, p_)) %
            p_;
        os.write(reinterpret_cast<const char *>(&result), sizeof(char));
    }
}

unsigned long ElGamal::p() const {
    return p_;
}

unsigned long ElGamal::g() const {
    return g_;
}

unsigned long ElGamal::d() const {
    return d_;
}

bool ElGamal::encrypt_file(const std::filesystem::path &in) {
    std::ifstream is(in, std::ios_base::binary);
    if (!is.is_open()) {
        return false;
    }

    ElGamal alice;
    ElGamal bob(alice.p(), alice.g());

    std::ofstream os(
        append_filename(in, "_ElGamalEncrypt"), std::ios_base::binary);
    alice.encrypt(is, os, bob.d());
    is.close();
    os.close();

    is.open(append_filename(in, "_ElGamalEncrypt"), std::ios_base::binary);
    os.open(append_filename(in, "_ElGamalDecrypt"), std::ios_base::binary);
    bob.decrypt(is, os);

    return true;
}

} // namespace cga::encryption