#include <libcga/mental_poker/user.hpp>

#include <libcga/base_functions/base_functions.hpp>

#include <algorithm>

extern const unsigned long c_max_gen_value;

namespace cga::mp {

User::User(unsigned long p) : _p(p) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<unsigned long> dist(2, _p - 2);
    do {
        _c = dist(gen);
    } while (cga::base_functions::extended_euclidean(
                 static_cast<long>(_p - 1), static_cast<long>(_c))[0] != 1);
    const long d = cga::base_functions::extended_euclidean(
        static_cast<long>(_p - 1), static_cast<long>(_c))[2];
    _d = d < 0 ? d + _p - 1 : d;
}

void User::encrypt(Items &items) const {
    std::mt19937 gen(std::random_device{}());
    for (auto &item : items) {
        item = cga::base_functions::modular_exponentiation(item, _c, _p);
    }
    std::shuffle(items.begin(), items.end(), gen);
}

void User::decrypt(Items &items) const {
    for (auto &item : items) {
        item = cga::base_functions::modular_exponentiation(item, _d, _p);
    }
}

void User::set_items(const Items &items) {
    _items = items;
}

void User::set_items(Items &&items) {
    _items = std::move(items);
}

Items &User::items() {
    return _items;
}

const Items &User::items() const {
    return _items;
}

} // namespace cga::mp
