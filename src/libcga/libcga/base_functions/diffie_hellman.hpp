#pragma once

namespace cga::base_functions::dh {

void generate_shared_data(unsigned long &P, unsigned long &g);
void generate_keys(
    unsigned long &X, unsigned long &Y, unsigned long P, unsigned long g);
unsigned long
calc_private_key(unsigned long Y, unsigned long X, unsigned long P);

} // namespace cga::base_functions::dh