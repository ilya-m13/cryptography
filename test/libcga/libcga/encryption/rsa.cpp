#include <gtest/gtest.h>

#include <libcga/encryption/rsa.hpp>

#include <fstream>

TEST(RSA, Encrypt) {
    cga::encryption::RSA alice;
    cga::encryption::RSA bob;

    const unsigned long correct = bob.N() - 1;

    unsigned long result = alice.encrypt(correct, bob.d(), bob.N());
    result = bob.decrypt(result);

    EXPECT_EQ(result, correct);
}

TEST(RSA, EncryptFile) {
    std::filesystem::current_path(std::filesystem::temp_directory_path());
    std::filesystem::create_directory("RSA");
    std::filesystem::path in =
        std::filesystem::current_path() / "RSA" / "file.bin";
    std::filesystem::path out =
        std::filesystem::current_path() / "RSA" / "file_RSAdecrypt.bin";

    const std::string correct = "1111111111111111111111111111111";
    std::string result;

    std::fstream ios(in, std::ios_base::binary | std::ios_base::out);
    ios << correct;
    ios.close();

    ASSERT_TRUE(cga::encryption::RSA::encrypt_file(in));

    ios.open(out, std::ios_base::binary | std::ios_base::in);
    ios >> result;

    EXPECT_STREQ(result.c_str(), correct.c_str());
}