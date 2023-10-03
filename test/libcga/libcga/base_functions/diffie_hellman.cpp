#include <gtest/gtest.h>

#include <libcga/base_functions/diffie_hellman.hpp>

// NOLINTBEGIN(cppcoreguidelines-init-variables)
// NOLINTBEGIN(readability-isolate-declaration)

TEST(DfKeyExchange, test) {
    unsigned long P, g, Xa, Ya, Xb, Yb, Zab, Zba;
    cga::base_functions::dh::generate_shared_data(P, g);
    cga::base_functions::dh::generate_keys(Xa, Ya, P, g);
    cga::base_functions::dh::generate_keys(Xb, Yb, P, g);
    Zab = cga::base_functions::dh::calc_private_key(Yb, Xa, P);
    Zba = cga::base_functions::dh::calc_private_key(Ya, Xb, P);

    EXPECT_EQ(Zab, Zba);
}

// NOLINTEND(readability-isolate-declaration)
// NOLINTEND(cppcoreguidelines-init-variables)