#include <libcga/mental_poker/table.hpp>

#include <libcga/base_functions/base_functions.hpp>

const unsigned long c_max_gen_value = 1000000000;
const unsigned long k = 100;

namespace cga::mp {

// usize * isizepu + isizet <= isize
Table::Table(
    unsigned long usize,
    unsigned long isize,
    unsigned long isizepu,
    unsigned long isizet)
    : _isizepu(isizepu) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<unsigned long> dist(5, c_max_gen_value);

    unsigned long q = 0;
    do {
        _p = dist(gen);
        q = (_p - 1) / 2;
    } while (_p % 2 == 0 || !cga::base_functions::miller_rabin_test(_p, k) ||
             !cga::base_functions::miller_rabin_test(q, k));

    _users.resize(usize, User{_p});

    _items.resize(isize);
    std::iota(_items.begin(), _items.end(), 2);

    _titems.resize(isizet);
}

void Table::distribute() {
    Items items = _items;

    for (const auto &user : _users) {
        user.encrypt(items);
    }

    auto begin_it = items.begin();
    auto end_it = begin_it + static_cast<long>(_isizepu);
    for (auto &user : _users) {
        Items items_per_user(_isizepu);
        std::move(begin_it, end_it, items_per_user.begin());
        begin_it = end_it;
        end_it = begin_it + static_cast<long>(_isizepu);

        user.set_items(std::move(items_per_user));
    }

    end_it = begin_it + static_cast<long>(_titems.size());
    std::move(begin_it, end_it, _titems.begin());

    for (auto &user1 : _users) {
        for (const auto &user2 : _users) {
            if (&user1 != &user2) {
                user2.decrypt(user1.items());
            }
        }
        user1.decrypt(user1.items());
    }

    for (const auto &user : _users) {
        user.decrypt(_titems);
    }
}

void Table::print(std::ostream &os) const {
    os << "Table: ";
    for (const auto &item : _titems) {
        os << item << " ";
    }
    os << "\n\n";

    for (unsigned long i = 0; i < _users.size(); i++) {
        os << "User[" << i << "]: ";
        for (const auto &item : _users[i].items()) {
            os << item << " ";
        }
        os << '\n';
    }
}

const Users &Table::users() const {
    return _users;
}

const Items &Table::items() const {
    return _items;
}

const Items &Table::titems() const {
    return _titems;
}

} // namespace cga::mp