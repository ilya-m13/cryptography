#include <libcga/base_functions/diffie_hellman.hpp>

#include <libcga/base_functions/base_functions.hpp>

#include <limits>
#include <random>

const unsigned long k = 100;
const unsigned long max_gen_value = 1000000000;

namespace cga::base_functions::dh {

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
    } while (P % 2 == 0 || P < 5 ||
             !cga::base_functions::miller_rabin_test(P, k) ||
             !cga::base_functions::miller_rabin_test(Q, k));

    do {
        g = dist(gen) % P;
    } while (g < 2 &&
             cga::base_functions::modular_exponentiation(g, Q, P) == 1);
}

void generate_keys(
    unsigned long &X, unsigned long &Y, unsigned long P, unsigned long g) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long> dist(1, P - 1);

    X = dist(gen);
    Y = cga::base_functions::modular_exponentiation(g, X, P);
}

unsigned long
calc_private_key(unsigned long Y, unsigned long X, unsigned long P) {
    return cga::base_functions::modular_exponentiation(Y, X, P);
}

} // namespace cga::base_functions::dh