#include <libcga/signature/elgamal.hpp>
#include <libcga/signature/gost.hpp>
#include <libcga/signature/rsa.hpp>

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

    cga::signature::RSA::sign_file(filepath);
    cga::signature::ElGamal::sign_file(filepath);
    cga::signature::GOST::sign_file(filepath);
}