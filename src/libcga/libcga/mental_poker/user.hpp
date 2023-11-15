#pragma once

#include <random>
#include <vector>

namespace cga::mp {

using Items = std::vector<unsigned long>;

class User {
  public:
    User() = delete;
    explicit User(unsigned long p);

    void encrypt(Items &items) const;
    void decrypt(Items &items) const;

    void set_items(const Items &items);
    void set_items(Items &&items);

    Items &items();
    const Items &items() const;

  private:
    unsigned long _c;
    unsigned long _d;
    unsigned long _p;

    Items _items;
};

} // namespace cga::mp