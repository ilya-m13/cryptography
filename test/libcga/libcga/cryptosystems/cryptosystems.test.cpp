#include <gtest/gtest.h>

#include <libcga/cryptosystems/cryptosystems.hpp>

#include <iostream>

// NOLINTBEGIN(fuchsia-default-arguments-calls)

TEST(ModularExponentiation, test) {
    EXPECT_EQ(
        cga::cryptosystems::modular_exponentiation(5274584, 2745723, 750037),
        721090);
}

TEST(ExtendedEuclidean, test) {
    auto result = cga::cryptosystems::extended_euclidean(2745721, 1634950);
    std::vector<int> correct = {1, -420519, 706216};
    EXPECT_TRUE(result == correct);
}

TEST(BabyStepGiantStep, test) {
    EXPECT_EQ(
        cga::cryptosystems::baby_step_giant_step(5274584, 721090, 750037),
        495615);
}

// NOLINTEND(fuchsia-default-arguments-calls)