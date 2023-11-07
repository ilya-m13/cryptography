#pragma once

#include <filesystem>
#include <vector>

namespace cga::signature {

class GOST {
  public:
    GOST();
    GOST(unsigned int p, unsigned short q, unsigned int a);
    GOST(
        unsigned int p,
        unsigned short q,
        unsigned int a,
        unsigned int x,
        unsigned int y)
        : p_(p), q_(q), a_(a), x_(x), y_(y) {}

    std::pair<unsigned short, std::vector<unsigned short>>
    sign(const char *m, std::size_t size) const;
    bool verify(
        const std::pair<unsigned short, std::vector<unsigned short>> &pair,
        const char *m,
        std::size_t size,
        unsigned int y) const;

    unsigned int p() const;
    unsigned short q() const;
    unsigned int a() const;
    unsigned int y() const;

    static bool sign_file(const std::filesystem::path &in);

  private:
    unsigned int p_;
    unsigned short q_;
    unsigned int a_;
    unsigned int x_;
    unsigned int y_;
};

} // namespace cga::signature