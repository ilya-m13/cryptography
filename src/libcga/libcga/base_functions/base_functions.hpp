#pragma once

#include <vector>

namespace cga::base_functions {

unsigned long
modular_exponentiation(unsigned long a, unsigned long x, unsigned long p);
std::vector<long> extended_euclidean(long a, long b);
unsigned long
baby_step_giant_step(unsigned long a, unsigned long y, unsigned long p);
bool miller_rabin_test(unsigned long n, unsigned long k);

} // namespace cga::base_functions