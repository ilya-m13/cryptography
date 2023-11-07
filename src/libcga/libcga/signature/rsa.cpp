#include <libcga/signature/rsa.hpp>

#include <libcga/base_functions/base_functions.hpp>

#include <cryptopp/sha.h>

#include <cassert>
#include <fstream>
#include <iterator>
#include <random>
#include <string>

const unsigned long max_gen_value = 10000;
const unsigned long k = 100;

static std::filesystem::path append_filename(
    const std::filesystem::path &in, const std::filesystem::path &name) {
    auto out = in;
    out.replace_filename(in.stem() += name);
    out.replace_extension(in.extension());
    return out;
}

namespace cga::signature {

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

std::vector<unsigned long> RSA::sign(const char *m, std::size_t size) const {
    CryptoPP::SHA256 sha256;

    std::vector<unsigned long> s(sha256.DigestSize());
    std::vector<unsigned char> hash(sha256.DigestSize());

    sha256.CalculateDigest(
        reinterpret_cast<CryptoPP::byte *>(hash.data()),
        reinterpret_cast<const CryptoPP::byte *>(m),
        size);

    for (unsigned long i = 0; i < sha256.DigestSize(); i++) {
        s[i] = cga::base_functions::modular_exponentiation(hash[i], c_, N_);
    }

    return s;
}

bool RSA::verify(
    const std::vector<unsigned long> &s,
    const char *m,
    std::size_t size,
    unsigned long d,
    unsigned long N) const {
    CryptoPP::SHA256 sha256;

    if (sha256.DigestSize() != s.size()) {
        return false;
    }

    std::vector<unsigned char> hash(sha256.DigestSize());
    std::vector<unsigned long> w(sha256.DigestSize());

    sha256.CalculateDigest(
        reinterpret_cast<CryptoPP::byte *>(hash.data()),
        reinterpret_cast<const CryptoPP::byte *>(m),
        size);

    for (std::size_t i = 0; i < sha256.DigestSize(); ++i) {
        w[i] = cga::base_functions::modular_exponentiation(s[i], d, N);
    }

    std::vector<unsigned long> hash_vec(hash.begin(), hash.end());
    return w == hash_vec;
}

unsigned long RSA::N() const {
    return N_;
}

unsigned long RSA::d() const {
    return d_;
}

bool RSA::sign_file(const std::filesystem::path &in) {
    std::ifstream is(in, std::ios_base::binary);
    if (!is.is_open()) {
        return false;
    }

    RSA alice;
    RSA bob;
    std::ofstream os(append_filename(in, "_RSAsign"), std::ios_base::binary);

    std::string str(
        std::istreambuf_iterator<char>{is}, std::istreambuf_iterator<char>{});
    const auto sign = alice.sign(str.c_str(), str.size());
    os.write(
        reinterpret_cast<const char *>(sign.data()),
        static_cast<std::streamsize>(sizeof(sign[0]) * sign.size()));
    assert(bob.verify(sign, str.c_str(), str.size(), alice.d(), alice.N()));

    return true;
}

} // namespace cga::signature
