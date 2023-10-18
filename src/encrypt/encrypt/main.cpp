#include <libcga/encryption/elgamal.hpp>
#include <libcga/encryption/rsa.hpp>
#include <libcga/encryption/shamir.hpp>
#include <libcga/encryption/vernam.hpp>

#include <cxxopts.hpp>

#include <iostream>

const std::string c_path_opt = "path";

int main(int argc, char **argv) {
    cxxopts::Options options("main");
    options.positional_help("<filepath>");
    // clang-format off
    options.add_options()
        (c_path_opt, "", cxxopts::value<std::filesystem::path>())
        ("h,help", "")
    ;
    // clang-format on
    options.parse_positional(c_path_opt);
    const auto result = options.parse(argc, argv);
    if (result.count(c_path_opt) != 1 || result.count("help") > 0) {
        std::cout << options.help() << "\n";
        return 0;
    }

    const auto filepath = result[c_path_opt].as<std::filesystem::path>();

    cga::encryption::ElGamal::encrypt_file(filepath);
    cga::encryption::Vernam::encrypt_file(filepath);
    cga::encryption::Shamir::encrypt_file(filepath);
    cga::encryption::RSA::encrypt_file(filepath);
}