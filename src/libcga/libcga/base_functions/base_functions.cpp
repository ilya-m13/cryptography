#include <libcga/base_functions/base_functions.hpp>

#include <cmath>
#include <random>
#include <unordered_map>
#include <vector>

namespace cga::base_functions {

unsigned long
modular_exponentiation(unsigned long a, unsigned long x, unsigned long p) {
    if (x == 0) {
        return 1;
    }

    unsigned long y = 1;
    unsigned long s = a;
    unsigned long t = std::floor(std::log2(x));

    for (unsigned long i = 0; i <= t; ++i) {
        unsigned long x_i = (x >> i) & 1U;
        if (x_i == 1) {
            y = y * s % p;
        }
        s = s * s % p;
    }

    return y;
}

// a >= b
std::vector<int> extended_euclidean(int a, int b) {
    std::vector<int> U = {a, 1, 0};
    std::vector<int> V = {b, 0, 1};
    std::vector<int> T(3);

    while (V[0] != 0) {
        int q = U[0] / V[0];
        T = {U[0] % V[0], U[1] - q * V[1], U[2] - q * V[2]};
        U = std::move(V);
        V = std::move(T);
    }

    return U;
}

unsigned long
baby_step_giant_step(unsigned long a, unsigned long y, unsigned long p) {
    unsigned long m, k;
    m = k = std::ceil(std::sqrt(p));

    std::unordered_map<unsigned long, unsigned long> A(k);
    std::vector<unsigned long> B(m);

    const unsigned long tmp = y % p;
    for (unsigned long i = 0; i < m; ++i) {
        A[cga::base_functions::modular_exponentiation(a, (i + 1) * m, p)] =
            i + 1;
        B[i] = cga::base_functions::modular_exponentiation(a, i, p) * tmp % p;
    }

    for (unsigned long i = 0; i < k; ++i) {
        if (A.contains(B[i])) {
            return A[B[i]] * m - i;
        }
    }

    return -1;
}

bool miller_rabin_test(unsigned long n, unsigned long k) {
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

        unsigned long x = cga::base_functions::modular_exponentiation(a, t, n);
        if (x == 1 || x == n - 1) {
            continue;
        }

        for (unsigned long r = 1; r < s; ++r) {
            x = cga::base_functions::modular_exponentiation(x, 2, n);
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

} // namespace cga::base_functions