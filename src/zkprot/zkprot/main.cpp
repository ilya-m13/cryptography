#include <libcga/zkprot/parser.hpp>
#include <libcga/zkprot/user.hpp>

#include <cxxopts.hpp>

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

const std::string c_filepath_opt = "filepath";
const unsigned int c_t = 5U;

int main(int argc, char **argv) {
    cxxopts::Options options("zkprot");
    options.positional_help("<matrix file>");
    // clang-format off
    options.add_options()
        (c_filepath_opt, "matrix file path", cxxopts::value<std::string>())
        ("help", "")
    ;
    // clang-format on
    options.parse_positional(c_filepath_opt);
    const auto result = options.parse(argc, argv);
    if (result.count(c_filepath_opt) != 1 || result.count("help") > 0) {
        std::cout << options.help() << '\n';
        return 0;
    }

    const std::string filepath = result[c_filepath_opt].as<std::string>();

    std::ifstream ifs(filepath);
    if (!ifs.good()) {
        std::cout << "Unable to read file\n";
        return 1;
    }

    const auto pair = cga::zkprot::parse(ifs);
    cga::zkprot::User alice(pair.first, pair.second);
    cga::zkprot::User bob(pair.first);

    for (std::size_t i = 0; i < c_t; i++) {
        if (i % 2 == 0) {
            assert(bob.verify(alice.what_hpath(), alice.d(), alice.n()));
        } else {
            assert(bob.verify(alice.is_isomorphic(), alice.d(), alice.n()));
        }
    }
}