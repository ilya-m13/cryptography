#include <libcga/signature/gost.hpp>

#include <libcga/base_functions/base_functions.hpp>

#include <cryptopp/sha.h>

#include <cassert>
#include <fstream>
#include <limits>
#include <random>

const unsigned short c_min_q = 1U << 15U;
const unsigned short c_max_q = std::numeric_limits<unsigned short>::max();

const unsigned int c_min_p = 1U << 31U;
const unsigned int c_max_p = std::numeric_limits<unsigned int>::max();

const unsigned long k = 100;

static std::filesystem::path append_filename(
    const std::filesystem::path &in, const std::filesystem::path &name) {
    auto out = in;
    out.replace_filename(in.stem() += name);
    out.replace_extension(in.extension());
    return out;
}

namespace cga::signature {

GOST::GOST() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned short> dist_q(c_min_q, c_max_q);

    unsigned int b = 0;
    while (true) {
        do {
            q_ = dist_q(gen);
        } while (!cga::base_functions::miller_rabin_test(q_, k));

        const unsigned int min_b = c_min_p / q_ + 1;
        const unsigned int max_b = c_max_p / q_;

        for (std::size_t i = min_b; i <= max_b; i++) {
            p_ = i * q_ + 1;
            if (cga::base_functions::miller_rabin_test(p_, k)) {
                b = i;
                break;
            }
        }

        if (b == 0) {
            continue;
        }
        break;
    }

    std::uniform_int_distribution<unsigned int> dist_g(2, p_ - 2);
    do {
        const unsigned int g = dist_g(gen);
        a_ = cga::base_functions::modular_exponentiation(g, b, p_);
    } while (a_ <= 1);

    std::uniform_int_distribution<unsigned short> dist_x(1, q_ - 1);
    x_ = dist_x(gen);
    y_ = cga::base_functions::modular_exponentiation(a_, x_, p_);
}

GOST::GOST(unsigned int p, unsigned short q, unsigned int a)
    : p_(p), q_(q), a_(a) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned short> dist_x(1, q_ - 1);
    x_ = dist_x(gen);
    y_ = cga::base_functions::modular_exponentiation(a_, x_, p_);
}

std::pair<unsigned short, std::vector<unsigned short>>
GOST::sign(const char *m, std::size_t size) const {
    CryptoPP::SHA256 sha256;

    std::vector<unsigned char> hash(sha256.DigestSize());
    std::vector<unsigned short> s(sha256.DigestSize());
    unsigned short r = 0;

    sha256.CalculateDigest(
        reinterpret_cast<CryptoPP::byte *>(hash.data()),
        reinterpret_cast<const CryptoPP::byte *>(m),
        size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned short> dist(1, q_ - 1);

    while (true) {
        unsigned short k = dist(gen);

        r = cga::base_functions::modular_exponentiation(a_, k, p_) % q_;
        if (r == 0) {
            continue;
        }

        bool zero_digits = true;
        for (std::size_t i = 0; i < sha256.DigestSize(); i++) {
            s[i] = (k * hash[i] + x_ * r) % q_;
        }
        for (unsigned short el : s) {
            if (el != 0) {
                zero_digits = false;
                break;
            }
        }
        if (zero_digits) {
            continue;
        }

        break;
    }

    return {r, s};
}

bool GOST::verify(
    const std::pair<unsigned short, std::vector<unsigned short>> &pair,
    const char *m,
    std::size_t size,
    unsigned int y) const {
    CryptoPP::SHA256 sha256;

    if (pair.second.size() != sha256.DigestSize()) {
        return false;
    }

    std::vector<unsigned char> hash(sha256.DigestSize());
    std::vector<unsigned short> hash_1(sha256.DigestSize());
    std::vector<unsigned short> u1(sha256.DigestSize());
    std::vector<unsigned short> u2(sha256.DigestSize());
    std::vector<unsigned short> v(sha256.DigestSize());

    sha256.CalculateDigest(
        reinterpret_cast<CryptoPP::byte *>(hash.data()),
        reinterpret_cast<const CryptoPP::byte *>(m),
        size);

    if (pair.first == 0 || pair.first >= q_) {
        return false;
    }
    for (unsigned short el : pair.second) {
        if (el >= q_) {
            return false;
        }
    }

    for (std::size_t i = 0; i < sha256.DigestSize(); i++) {
        long value1 = cga::base_functions::extended_euclidean(q_, hash[i])[2];
        hash_1[i] = value1 < 0 ? value1 + q_ : value1;
        u1[i] = (static_cast<unsigned long>(pair.second[i]) *
                 static_cast<unsigned long>(hash_1[i])) %
            q_;
        long value2 =
            (static_cast<long>(-pair.first) * static_cast<long>(hash_1[i])) %
            static_cast<long>(q_);
        u2[i] = value2 < 0 ? value2 + q_ : value2;
        v[i] = ((cga::base_functions::modular_exponentiation(a_, u1[i], p_) *
                 cga::base_functions::modular_exponentiation(y, u2[i], p_)) %
                p_) %
            q_;
        if (v[i] != pair.first) {
            return false;
        }
    }

    return true;
}

unsigned int GOST::p() const {
    return p_;
}

unsigned short GOST::q() const {
    return q_;
}

unsigned int GOST::a() const {
    return a_;
}

unsigned int GOST::y() const {
    return y_;
}

bool GOST::sign_file(const std::filesystem::path &in) {
    std::ifstream is(in, std::ios_base::binary);
    if (!is.is_open()) {
        return false;
    }

    GOST alice;
    std::ofstream os(append_filename(in, "_GOSTsign"), std::ios_base::binary);

    std::string str(
        std::istreambuf_iterator<char>{is}, std::istreambuf_iterator<char>{});
    const auto sign = alice.sign(str.c_str(), str.size());
    os.write(reinterpret_cast<const char *>(&sign.first), sizeof(sign.first));
    os.write(
        reinterpret_cast<const char *>(sign.second.data()),
        static_cast<std::streamsize>(
            sizeof(sign.second[0]) * sign.second.size()));
    assert(alice.verify(sign, str.c_str(), str.size(), alice.y()));

    return true;
}

} // namespace cga::signature