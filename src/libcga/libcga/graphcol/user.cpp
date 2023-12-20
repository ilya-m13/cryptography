#include <libcga/graphcol/user.hpp>

#include <libcga/base_functions/base_functions.hpp>

#include <algorithm>
#include <array>
#include <limits>
#include <numeric>
#include <random>

const unsigned long c_k = 100UL;
const unsigned long c_max_gen_rsa_value = 10000UL;

namespace cga::graphcol {

User::User(EdgeVec edgevec) : _edgevec(std::move(edgevec)) {
    _c.resize(_colorvec.size());
}

User::User(EdgeVec edgevec, ColorVec colorvec)
    : _edgevec(std::move(edgevec)), _colorvec(std::move(colorvec)) {
    _c.resize(_colorvec.size());
}

User::VerData User::request() {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<unsigned long> dist(2, c_max_gen_rsa_value);

    std::shuffle(_color_pmt.begin(), _color_pmt.end(), gen);

    VerData verdata;
    verdata.d_.resize(_c.size());
    verdata.n_.resize(_c.size());
    verdata.z_.resize(_c.size());
    std::vector<unsigned long> r(_c.size());

    for (std::size_t i = 0; i < _colorvec.size(); i++) {
        unsigned long p = 0;
        unsigned long q = 0;
        do {
            p = dist(gen);
        } while (!cga::base_functions::miller_rabin_test(p, c_k));
        do {
            q = dist(gen);
        } while (!cga::base_functions::miller_rabin_test(q, c_k));

        verdata.n_[i] = p * q;

        const unsigned long phi = (p - 1) * (q - 1);
        std::uniform_int_distribution<unsigned long> key_dist(2, phi);
        do {
            verdata.d_[i] = key_dist(gen);
        } while (cga::base_functions::extended_euclidean(
                     static_cast<long>(phi),
                     static_cast<long>(verdata.d_[i]))[0] != 1);
        const long y = cga::base_functions::extended_euclidean(
            static_cast<long>(phi), static_cast<long>(verdata.d_[i]))[2];
        _c[i] = y < 0 ? y + phi : y;

        std::uniform_int_distribution<unsigned long> r_dist(
            2, verdata.n_[i] - 2);
        r[i] = r_dist(gen);
        unsigned short new_color = 0;
        for (std::size_t j = 0; j < _color_pmt.size(); j++) {
            if (_color_pmt[j] == _colorvec[i]) {
                if (j != 0) {
                    new_color = _color_pmt[j - 1];
                } else {
                    new_color = _color_pmt.back();
                }
                break;
            }
        }
        r[i] = (r[i] & (std::numeric_limits<unsigned long>::max() - 3UL)) +
            new_color;

        verdata.z_[i] = cga::base_functions::modular_exponentiation(
            r[i], verdata.d_[i], verdata.n_[i]);
    }

    return verdata;
}

std::pair<unsigned long, unsigned long>
User::select_edge(const Edge &edge) const {
    return {_c[edge.ver1_], _c[edge.ver2_]};
}

Edge User::select_edge() {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<std::size_t> dist(0, _edgevec.size() - 1);
    selected_edge = _edgevec[dist(gen)];
    return selected_edge;
}

bool User::verify(
    const User::VerData &verdata,
    std::pair<unsigned long, unsigned long> en_edge) const {
    const unsigned long z1 = cga::base_functions::modular_exponentiation(
                                 verdata.z_[selected_edge.ver1_],
                                 en_edge.first,
                                 verdata.n_[selected_edge.ver1_]) &
        3UL;
    const unsigned long z2 = cga::base_functions::modular_exponentiation(
                                 verdata.z_[selected_edge.ver2_],
                                 en_edge.second,
                                 verdata.n_[selected_edge.ver2_]) &
        3UL;

    return z1 != z2;
}

} // namespace cga::graphcol