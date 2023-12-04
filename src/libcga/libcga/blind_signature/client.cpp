#include <libcga/blind_signature/client.hpp>

#include <libcga/base_functions/base_functions.hpp>

#include <cryptopp/sha.h>

#include <random>

namespace cga::blindsign {

std::vector<unsigned long> Client::vote(unsigned long d, unsigned long n) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<unsigned long> dist(1, 1U << 31U);

    const unsigned long u = 0;
    const unsigned long rnd = dist(gen);
    _n = (rnd << 32U) + u;

    std::uniform_int_distribution<unsigned long> r_dist(2, n - 2);
    do {
        _r = r_dist(gen);
    } while (cga::base_functions::extended_euclidean(
                 static_cast<long>(n), static_cast<long>(_r))[0] != 1);

    CryptoPP::SHA256 sha256;
    std::vector<unsigned char> hash(sha256.DigestSize());
    std::vector<unsigned long> en_hash(sha256.DigestSize());
    sha256.CalculateDigest(
        reinterpret_cast<CryptoPP::byte *>(hash.data()),
        reinterpret_cast<CryptoPP::byte *>(&_n),
        sizeof(_n));

    const auto value = cga::base_functions::modular_exponentiation(_r, d, n);
    for (std::size_t i = 0; i < hash.size(); i++) {
        en_hash[i] = (hash[i] * value) % n;
    }

    return en_hash;
}

std::pair<unsigned long, std::vector<unsigned long>>
Client::bulletin(const std::vector<unsigned long> &s, unsigned long n) const {
    const long sign_r_1 = cga::base_functions::extended_euclidean(
        static_cast<long>(n), static_cast<long>(_r))[2];
    const unsigned long r_1 = sign_r_1 < 0 ? sign_r_1 + n : sign_r_1;

    std::vector<unsigned long> new_s(s.size());
    for (std::size_t i = 0; i < new_s.size(); i++) {
        new_s[i] = (s[i] * r_1) % n;
    }

    return {_n, new_s};
}

} // namespace cga::blindsign