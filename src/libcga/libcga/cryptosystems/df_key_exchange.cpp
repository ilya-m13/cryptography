#include <libcga/cryptosystems/df_key_exchange.hpp>

#include <libcga/cryptosystems/cryptosystems.hpp>

#include <limits>
#include <random>

const unsigned long k = 100;
const unsigned long max_gen_value = 1000000000;

static bool miller_rabin_test(unsigned long n, unsigned long k) {
    if (n == 2 || n == 3) {
        return true;
    }
    if (n < 2 || n % 2 == 0) {
        return false;
    }

    unsigned long t = n - 1;
    unsigned long s = 0;
    while (t % 2 == 0) {
        t /= 2;
        ++s;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long> dist(2, n - 3);
    for (unsigned long i = 0; i < k; ++i) {
        unsigned long a = dist(gen);

        unsigned long x = cga::cryptosystems::modular_exponentiation(a, t, n);
        if (x == 1 || x == n - 1) {
            continue;
        }

        for (unsigned long r = 1; r < s; ++r) {
            x = cga::cryptosystems::modular_exponentiation(x, 2, n);
            if (x == 1) {
                return false;
            }
            if (x == n - 1) {
                break;
            }
        }

        if (x != n - 1) {
            return false;
        }
    }

    return true;
}

namespace cga::cryptosystems::df {

void generate_shared_data(unsigned long &P, unsigned long &g) {
    std::random_device rd;
    std::mt19937 gen(rd());
    // std::uniform_int_distribution<unsigned long> dist(
    //     2, std::numeric_limits<unsigned long>::max());
    std::uniform_int_distribution<unsigned long> dist(2, max_gen_value);

    unsigned long Q = 0;
    do {
        P = dist(gen);
        Q = (P - 1) / 2;
    } while (P % 2 == 0 || P < 5 || !miller_rabin_test(P, k) ||
             !miller_rabin_test(Q, k));

    do {
        g = dist(gen) % P;
    } while (cga::cryptosystems::modular_exponentiation(g, Q, P) == 1);
}

void generate_keys(
    unsigned long &X, unsigned long &Y, unsigned long P, unsigned long g) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long> dist(1, P - 1);

    X = dist(gen);
    Y = cga::cryptosystems::modular_exponentiation(g, X, P);
}

unsigned long
calc_private_key(unsigned long Y, unsigned long X, unsigned long P) {
    return cga::cryptosystems::modular_exponentiation(Y, X, P);
}

} // namespace cga::cryptosystems::df