#include "../../strings/rolling-hash.hpp"

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_14_B&lang=jp

using mods_seq = std::integer_sequence<ll, static_cast<ll>(1e9 + 7), static_cast<ll>(1e9 + 9), 9997451>;
auto builder = strings::gen_rhbuilder(mods_seq());

int main() {
    std::string s, t;
    std::cin >> s >> t;
    auto sh = builder.build(ALL(s));
    auto th = builder.build(ALL(t)).back();
    for (ll i = 0; i + t.size() <= s.size(); i++) {
        auto sh_i = builder.substr(sh, i, i + t.size());
        if (th == sh_i) std::cout << i << '\n';
    }
    return 0;
}
