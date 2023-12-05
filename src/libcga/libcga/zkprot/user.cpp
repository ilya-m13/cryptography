#include <libcga/zkprot/user.hpp>

#include <libcga/base_functions/base_functions.hpp>

#include <algorithm>
#include <array>
#include <limits>
#include <numeric>
#include <random>

const unsigned long c_k = 100UL;
const unsigned long c_max_gen_rsa_value = 10000UL;

namespace cga::zkprot {

User::User(AdjMatrix g) : _g(std::move(g)) {
    set_initial_values();
}

User::User(AdjMatrix g, HamilPath hpath)
    : _g(std::move(g)), _hpath(std::move(hpath)) {
    set_initial_values();
}

User::PathVerData User::what_hpath() {
    PathVerData result = {._f = encrypt_mtx(), ._en_vers{}};

    std::vector<unsigned int> rvs_pmt(_pmt.size());
    for (std::size_t i = 0; i < rvs_pmt.size(); i++) {
        rvs_pmt[_pmt[i]] = i;
    }

    for (std::size_t i = 0; i < _hpath.size() - 1; i++) {
        const unsigned int v1 = rvs_pmt[_hpath[i]];
        const unsigned int v2 = rvs_pmt[_hpath[i + 1]];
        const unsigned long value = cga::base_functions::modular_exponentiation(
            result._f[v1][v2], _c, _n);
        result._en_vers.emplace_back(v1, v2, value);
    }
    {
        const unsigned int v1 = rvs_pmt[_hpath.back()];
        const unsigned int v2 = rvs_pmt[_hpath[0]];
        const unsigned long value = cga::base_functions::modular_exponentiation(
            result._f[v1][v2], _c, _n);
        result._en_vers.emplace_back(v1, v2, value);
    }

    return result;
}

User::IsomorphicVerData User::is_isomorphic() {
    IsomorphicVerData result = {._f = encrypt_mtx(), ._en_h{}, ._pmt = _pmt};
    result._en_h.resize(result._f.size());

    for (std::size_t i = 0; i < _g.size(); i++) {
        result._en_h[i].resize(_g.size());
        for (std::size_t j = 0; j < _g.size(); j++) {
            result._en_h[i][j] = cga::base_functions::modular_exponentiation(
                result._f[i][j], _c, _n);
        }
    }

    return result;
}

bool User::verify(
    const User::PathVerData &pathdata, unsigned long d, unsigned long n) const {
    std::vector<bool> vers(pathdata._f.size());
    for (const auto &en_ver : pathdata._en_vers) {
        const auto value =
            cga::base_functions::modular_exponentiation(en_ver._value, d, n);
        if (value != pathdata._f[en_ver._vers.first][en_ver._vers.second]) {
            return false;
        }

        if (vers[en_ver._vers.first]) {
            return false;
        }
        vers[en_ver._vers.first] = true;
    }

    return true;
}

bool User::verify(
    const User::IsomorphicVerData &isdata,
    unsigned long d,
    unsigned long n) const {
    Matrix h(isdata._en_h.size());
    for (std::size_t i = 0; i < isdata._f.size(); i++) {
        h[i].resize(isdata._en_h.size());
        for (std::size_t j = 0; j < isdata._f.size(); j++) {
            const auto value = cga::base_functions::modular_exponentiation(
                isdata._en_h[i][j], d, n);
            if (value != isdata._f[i][j]) {
                return false;
            }

            h[i][j] = isdata._en_h[i][j] & 1U;

            if (h[i][j] != _g[isdata._pmt[i]][isdata._pmt[j]]) {
                return false;
            }
        }
    }

    return true;
}

unsigned long User::d() const {
    return _d;
}

unsigned long User::n() const {
    return _n;
}

Matrix User::encrypt_mtx() {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<unsigned long> dist(2, _n - 2);

    std::shuffle(_pmt.begin(), _pmt.end(), gen);

    Matrix f(_g.size());
    for (std::size_t i = 0; i < _g.size(); i++) {
        f[i].resize(_g.size());
        for (std::size_t j = 0; j < _g.size(); j++) {
            f[i][j] =
                dist(gen) & (std::numeric_limits<unsigned long>::max() - 1);
            f[i][j] += _g[_pmt[i]][_pmt[j]];
            f[i][j] =
                cga::base_functions::modular_exponentiation(f[i][j], _d, _n);
        }
    }

    return f;
}

void User::set_initial_values() {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<unsigned long> dist(2, c_max_gen_rsa_value);

    do {
        _p = dist(gen);
    } while (!cga::base_functions::miller_rabin_test(_p, c_k));
    do {
        _q = dist(gen);
    } while (!cga::base_functions::miller_rabin_test(_q, c_k));
    _n = _p * _q;

    const unsigned long phi = (_p - 1) * (_q - 1);
    std::uniform_int_distribution<unsigned long> key_dist(2, phi);
    do {
        _d = key_dist(gen);
    } while (cga::base_functions::extended_euclidean(
                 static_cast<long>(phi), static_cast<long>(_d))[0] != 1);
    const long y = cga::base_functions::extended_euclidean(
        static_cast<long>(phi), static_cast<long>(_d))[2];
    _c = y < 0 ? y + phi : y;

    _pmt.resize(_g.size());
    std::iota(_pmt.begin(), _pmt.end(), 0U);
}

} // namespace cga::zkprot