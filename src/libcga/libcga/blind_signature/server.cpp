#include <libcga/blind_signature/server.hpp>

#include <libcga/base_functions/base_functions.hpp>

#include <cryptopp/sha.h>

#include <random>

const unsigned long c_max_gen_value = 10000;
const unsigned long c_k = 100;

namespace cga::blindsign {

Server::Server() {
    reset();
}

void Server::reset() {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<unsigned long> dist(2, c_max_gen_value);

    do {
        _p = dist(gen);
    } while (!cga::base_functions::miller_rabin_test(_p, c_k));
    do {
        _q = dist(gen);
    } while (!cga::base_functions::miller_rabin_test(_q, c_k));
    _n = _p * _q;

    unsigned long phi = (_p - 1) * (_q - 1);
    std::uniform_int_distribution<unsigned long> key_dist(2, phi);
    do {
        _d = key_dist(gen);
    } while (cga::base_functions::extended_euclidean(
                 static_cast<long>(phi), static_cast<long>(_d))[0] != 1);
    const long y = cga::base_functions::extended_euclidean(
        static_cast<long>(phi), static_cast<long>(_d))[2];
    _c = y < 0 ? y + phi : y;
}

std::vector<unsigned long>
Server::sign(const std::vector<unsigned long> &h) const {
    std::vector<unsigned long> new_s(h.size());
    for (std::size_t i = 0; i < new_s.size(); i++) {
        new_s[i] = cga::base_functions::modular_exponentiation(h[i], _c, _n);
    }
    return new_s;
}

bool Server::verify(
    unsigned long n, const std::vector<unsigned long> &s) const {
    CryptoPP::SHA256 sha256;

    std::vector<unsigned char> hash(sha256.DigestSize());
    std::vector<unsigned long> w(sha256.DigestSize());

    sha256.CalculateDigest(
        reinterpret_cast<CryptoPP::byte *>(hash.data()),
        reinterpret_cast<const CryptoPP::byte *>(&n),
        sizeof(n));

    for (std::size_t i = 0; i < w.size(); i++) {
        w[i] = cga::base_functions::modular_exponentiation(s[i], _d, _n);
    }

    for (std::size_t i = 0; i < hash.size(); i++) {
        if (hash[i] != w[i]) {
            return false;
        }
    }

    return true;
}

unsigned long Server::n() const {
    return _n;
}

unsigned long Server::d() const {
    return _d;
}

} // namespace cga::blindsign
