#include <gtest/gtest.h>

#include <libcga/encryption/elgamal.hpp>

#include <fstream>

TEST(ElGamal, Encrypt) {
    cga::encryption::ElGamal alice;
    cga::encryption::ElGamal bob(alice.p(), alice.g());

    const unsigned long result = alice.p() - 1;
    auto data = alice.encrypt(result, bob.d());
    unsigned long m = bob.decrypt(data);

    EXPECT_EQ(m, result);
}

TEST(ElGamal, EncryptFile) {
    std::filesystem::current_path(std::filesystem::temp_directory_path());
    std::filesystem::create_directory("ElGamal");
    const std::filesystem::path in(
        std::filesystem::current_path() / "ElGamal" / "file.bin");
    const std::filesystem::path out(
        std::filesystem::current_path() / "ElGamal" / "file_Decrypt.bin");

    std::ofstream os(in, std::ios_base::binary);
    std::string correct("111111111111111111111111111111111111111111111");
    os << correct;
    os.close();

    EXPECT_TRUE(cga::encryption::ElGamal::encrypt_file(in));

    std::string result;
    std::ifstream is(out, std::ios_base::binary);
    is >> result;
    EXPECT_STREQ(result.c_str(), correct.c_str());
}