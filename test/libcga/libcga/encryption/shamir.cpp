#include <gtest/gtest.h>

#include <libcga/encryption/shamir.hpp>

#include <fstream>

TEST(Shamir, Encrypt) {
    cga::encryption::Shamir alice;
    cga::encryption::Shamir bob(alice.P());

    const unsigned long result = 123456789UL;

    unsigned long value = alice.encrypt(result);
    value = bob.encrypt(value);
    value = alice.decrypt(value);
    value = bob.decrypt(value);

    EXPECT_EQ(result, value);
}

TEST(Shamir, EncryptFile) {
    std::filesystem::current_path(std::filesystem::temp_directory_path());
    std::filesystem::create_directory("Shamir");
    const std::filesystem::path in(
        std::filesystem::current_path() / "Shamir" / "file.bin");
    const std::filesystem::path out(
        std::filesystem::current_path() / "Shamir" / "file_Bde.bin");

    std::ofstream os(in, std::ios_base::binary);
    std::string correct("1111111111111111111111111111111111111111");
    os << correct;
    os.close();

    EXPECT_TRUE(cga::encryption::Shamir::encrypt_file(in));

    std::string result;
    std::ifstream is(out, std::ios_base::binary);
    is >> result;

    EXPECT_STREQ(result.c_str(), correct.c_str());
}