#include <libcga/mental_poker/table.hpp>

#include <cxxopts.hpp>

#include <iostream>
#include <string>

const std::string c_usize_opt = "usize";
const std::string c_isize_opt = "isize";
const std::string c_isizepu_opt = "isizepu";
const std::string c_isizet_opt = "isizet";

int main(int argc, char **argv) {
    cxxopts::Options options("mental poker");
    options.positional_help(
        "<usersize> <itemsize> <itemsize per user> <table item size>");
    // clang-format off
    options.add_options()
        (c_usize_opt, "user size", cxxopts::value<unsigned long>())
        (c_isize_opt, "item size", cxxopts::value<unsigned long>())
        (c_isizepu_opt, "itemsize per user", cxxopts::value<unsigned long>())
        (c_isizet_opt, "table item size", cxxopts::value<unsigned long>())
        ("h,help", "")
    ;
    // clang-format on
    options.parse_positional(
        {c_usize_opt, c_isize_opt, c_isizepu_opt, c_isizet_opt});
    const auto result = options.parse(argc, argv);
    if (result.count(c_usize_opt) != 1 || result.count(c_isize_opt) != 1 ||
        result.count(c_isizepu_opt) != 1 || result.count(c_isizet_opt) != 1 ||
        result.count("help") > 0) {
        std::cout << options.help() << "\n";
        return 0;
    }

    const unsigned long usize = result[c_usize_opt].as<unsigned long>();
    const unsigned long isize = result[c_isize_opt].as<unsigned long>();
    const unsigned long isizepu = result[c_isizepu_opt].as<unsigned long>();
    const unsigned long isizet = result[c_isizet_opt].as<unsigned long>();

    if (usize * isizepu + isizet > isize) {
        std::cerr << "usize * isizepu + isizet > isize\n";
        return 1;
    }

    cga::mp::Table table(usize, isize, isizepu, isizet);
    table.distribute();
    table.print(std::cout);
}