#include <gtest/gtest.h>

#include <libcga/cryptosystems/df_key_exchange.hpp>

// NOLINTBEGIN(cppcoreguidelines-init-variables)
// NOLINTBEGIN(readability-isolate-declaration)

TEST(DfKeyExchange, test) {
    unsigned long P, g, Xa, Ya, Xb, Yb, Zab, Zba;
    cga::cryptosystems::df::generate_shared_data(P, g);
    cga::cryptosystems::df::generate_keys(Xa, Ya, P, g);
    cga::cryptosystems::df::generate_keys(Xb, Yb, P, g);
    Zab = cga::cryptosystems::df::calc_private_key(Yb, Xa, P);
    Zba = cga::cryptosystems::df::calc_private_key(Ya, Xb, P);

    EXPECT_EQ(Zab, Zba);
}

// NOLINTEND(readability-isolate-declaration)
// NOLINTEND(cppcoreguidelines-init-variables)