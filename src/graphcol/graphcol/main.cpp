#include <libcga/graphcol/parser.hpp>
#include <libcga/graphcol/user.hpp>

#include <cxxopts.hpp>

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

const std::string c_filepath_opt = "filepath";
const unsigned int c_a = 2;

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

    const auto pair = cga::graphcol::parse(ifs);
    cga::graphcol::User alice(pair.second, pair.first);
    cga::graphcol::User bob(pair.second);

    for (std::size_t i = 0; i < c_a * pair.second.size(); i++) {
        const auto verdata = alice.request();
        const auto en_edge = alice.select_edge(bob.select_edge());
        if (!bob.verify(verdata, en_edge)) {
            std::cout << "Алиса попыталась обмануть Боба\n";
            return 1;
        }
    }

    return 0;
}