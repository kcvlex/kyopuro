#pragma once
#include "../util/template.hpp"
#include "base.hpp"
#include "modint.hpp"

namespace math {

template <ll Mod>
class Factorial {
    using mint = Modint<Mod>;

    ssize_t maxv;
    vec<mint> factv, ifactv;

public:
    Factorial(ssize_t maxv) : maxv(maxv), factv(maxv + 1), ifactv(maxv + 1) {
        factv[0] = mint(1);
        for (ll i = 1; i <= maxv; i++) factv[i] = factv[i - 1] * i;
        ifactv.back() = factv.back().inv();
        for (ll i = maxv - 1; 0 <= i; i--) ifactv[i] = ifactv[i + 1] * (i + 1);
    }

    mint fact(ll n) const {
        return factv[n]; 
    }

    mint ifact(ll n) const {
        return ifactv[n];
    }

    mint perm(ll n, ll k) const {
        return factv[n] * ifactv[n - k];
    }

    mint comb(ll n, ll k) const {
        return perm(n, k) * ifactv[k];
    }
};

}