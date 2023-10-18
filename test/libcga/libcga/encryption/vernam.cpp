#include <gtest/gtest.h>

#include <libcga/encryption/vernam.hpp>

#include <fstream>

TEST(Vernam, Encrypt) {
    const unsigned char correct = 200;

    const auto pair = cga::encryption::Vernam::encrypt(correct);
    unsigned char result =
        cga::encryption::Vernam::decrypt(pair.first, pair.second);

    EXPECT_EQ(result, correct);
}

TEST(Vernam, EncryptFile) {
    std::filesystem::current_path(std::filesystem::temp_directory_path());
    std::filesystem::create_directory("VERNAM");
    std::filesystem::path in =
        std::filesystem::current_path() / "VERNAM" / "file.bin";
    std::filesystem::path out =
        std::filesystem::current_path() / "VERNAM" / "file_VernamDecrypt.bin";

    const std::string correct = "1111111111111111111111111111111";
    std::string result;

    std::fstream ios(in, std::ios_base::binary | std::ios_base::out);
    ios << correct;
    ios.close();

    ASSERT_TRUE(cga::encryption::Vernam::encrypt_file(in));

    ios.open(out, std::ios_base::binary | std::ios_base::in);
    ios >> result;

    EXPECT_STREQ(result.c_str(), correct.c_str());
}