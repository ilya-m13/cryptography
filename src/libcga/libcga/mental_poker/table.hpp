#pragma once

#include <libcga/mental_poker/user.hpp>

#include <ostream>

namespace cga::mp {

using Users = std::vector<User>;

class Table {
  public:
    Table() = delete;
    Table(
        unsigned long usize,
        unsigned long isize,
        unsigned long isizepu,
        unsigned long isizet);

    void distribute();
    void print(std::ostream &os) const;

    const Users &users() const;
    const Items &items() const;
    const Items &titems() const;

  private:
    unsigned long _p;

    unsigned long _isizepu;

    Users _users;
    Items _items;
    Items _titems;
};

} // namespace cga::mp