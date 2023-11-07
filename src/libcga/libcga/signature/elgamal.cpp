#include <libcga/signature/elgamal.hpp>

#include <libcga/base_functions/base_functions.hpp>

#include <cryptopp/sha.h>

#include <cassert>
#include <cmath>
#include <fstream>
#include <iterator>
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

namespace cga::signature {

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

std::pair<unsigned long, std::vector<unsigned long>>
ElGamal::sign(const char *m, std::size_t size) const {
    CryptoPP::SHA256 sha256;

    std::vector<unsigned char> hash(sha256.DigestSize());
    std::vector<unsigned long> s(sha256.DigestSize());

    sha256.CalculateDigest(
        reinterpret_cast<CryptoPP::byte *>(hash.data()),
        reinterpret_cast<const CryptoPP::byte *>(m),
        size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long> dist(2, p_ - 2);
    unsigned long k = 0;
    do {
        k = dist(gen);
    } while (cga::base_functions::extended_euclidean(
                 static_cast<long>(p_ - 1), static_cast<long>(k))[0] != 1);

    const unsigned long r =
        cga::base_functions::modular_exponentiation(g_, k, p_);

    const auto eu = cga::base_functions::extended_euclidean(
        static_cast<long>(p_ - 1), static_cast<long>(k))[2];
    const unsigned long k_1 = eu < 0 ? eu + p_ - 1 : eu;

    for (std::size_t i = 0; i < sha256.DigestSize(); i++) {
        const long value =
            (static_cast<long>(hash[i]) - static_cast<long>(c_ * r)) %
            static_cast<long>(p_ - 1);
        s[i] = value < 0 ? value + p_ - 1 : value;
        s[i] = k_1 * s[i] % (p_ - 1);
    }

    return {r, s};
}

bool ElGamal::verify(
    const std::pair<unsigned long, std::vector<unsigned long>> &pair,
    const char *m,
    std::size_t size,
    unsigned long d) const {
    CryptoPP::SHA256 sha256;

    std::vector<unsigned char> hash(sha256.DigestSize());

    sha256.CalculateDigest(
        reinterpret_cast<CryptoPP::byte *>(hash.data()),
        reinterpret_cast<const CryptoPP::byte *>(m),
        size);

    for (std::size_t i = 0; i < sha256.DigestSize(); i++) {
        const unsigned long l =
            cga::base_functions::modular_exponentiation(d, pair.first, p_) *
            cga::base_functions::modular_exponentiation(
                pair.first, pair.second[i], p_) %
            p_;
        const unsigned long r =
            cga::base_functions::modular_exponentiation(g_, hash[i], p_);
        if (l != r) {
            return false;
        }
    }

    return true;
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

bool ElGamal::sign_file(const std::filesystem::path &in) {
    std::ifstream is(in, std::ios_base::binary);
    if (!is.is_open()) {
        return false;
    }

    ElGamal alice;
    ElGamal bob(alice.p(), alice.g());
    std::ofstream os(
        append_filename(in, "_ElGamalsign"), std::ios_base::binary);

    std::string str(
        std::istreambuf_iterator<char>{is}, std::istreambuf_iterator<char>{});
    const auto sign = alice.sign(str.c_str(), str.size());
    os.write(reinterpret_cast<const char *>(&sign.first), sizeof(sign.first));
    os.write(
        reinterpret_cast<const char *>(sign.second.data()),
        static_cast<std::streamsize>(
            sizeof(sign.second[0]) * sign.second.size()));
    assert(bob.verify(sign, str.c_str(), str.size(), alice.d()));

    return true;
}

} // namespace cga::signature