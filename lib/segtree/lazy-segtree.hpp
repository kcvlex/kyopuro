#include "../util/template.hpp"
#include "../util/ceil-pow2.hpp"
#include "/home/taroy/kyopuro/lib/util/debug.hpp"

namespace segtree {

namespace {

struct has_id_ele {
    template <typename T>
    auto operator ()(T &&t) -> decltype(T::id_ele(), std::true_type()) { return std::true_type(); }
    std::false_type operator ()(...) { return std::false_type(); }
};

struct has_merge {
    template <typename T>
    auto operator ()(T &&t) -> decltype(T::merge(std::declval<T>(), std::declval<T>()), std::true_type()) { return std::true_type(); }
    std::false_type operator ()(...) { return std::false_type(); }
};

struct has_apply {
    template <typename M, typename Op>
    auto operator ()(M &&m, Op &&op) -> decltype(m.apply(op), std::true_type()) { return std::true_type(); }
    std::false_type operator ()(...) { return std::false_type(); }
};

template <typename F, typename... Args>
using callable = std::is_same<typename std::invoke_result<F, Args...>::type, std::true_type>;

template <typename M>
using is_monoid = std::conjunction<
    callable<has_id_ele, M>, callable<has_merge, M>>;

template <typename M, typename Op>
using enable_apply = callable<has_apply, M, Op>;

}

template <typename M, typename Op>
class LazySegmentTree {
    static_assert(is_monoid<M>::value, "M must be monoid.");
    static_assert(is_monoid<Op>::value, "Op must be monoid.");
    static_assert(enable_apply<M, Op>::value, "Op is not operator of M.");
    
    using size_type = ssize_t;

    struct segment {
        M m;
        Op op;
        bool has_lazy;

        segment(M m = M::id_ele()) : m(m), op(Op::id_ele()), has_lazy(false) { }

        void update_op(Op o) {
            op = Op::merge(op, o);
            has_lazy = true;
        }

        void apply() {
            if (!has_lazy) return;
            m.apply(op);
            init_op();
        }

        void init_op() {
            op = Op::id_ele();
            has_lazy = false;
        }
    };

    vec<segment> segs;
    size_type height;

    void push(size_type idx) {
        auto &seg = segs[idx];
        if (!seg.has_lazy) return;
        auto tmp = seg.op;
        seg.apply();
        for (int i = 0; i < 2; i++) {
            auto cidx = 2 * idx + i;
            if (2 * size() <= cidx) break;
            auto &cseg = segs[cidx];
            cseg.update_op(tmp);
        }
    }

    auto msb(size_type i) {
        return 32 - __builtin_clz(i) - 1;
    }

    void propagate_from_top(size_type idx) {
        for (int i = height; 1 <= i; i--) push(idx >> i);
    }

    void update_from_bottom(size_type idx) {
        while (true) {
            auto pidx = idx / 2;
            if (pidx == 0) break;
            push(2 * pidx);
            push(2 * pidx + 1);
            segs[pidx].m = M::merge(segs[2 * pidx].m, segs[2 * pidx + 1].m);
            idx = pidx;
        }
    }

    size_type get_endpoint_seg(size_type i) {
        i += size();
        return i / (i & -i);
    }

public:
    template <typename T>
    LazySegmentTree(const vec<T> &v) {
        size_type sz = ceil_pow2(v.size());
        segs.resize(sz * 2);
        height = msb(sz);
        for (auto i = sz; i < v.size(); i++) segs[i] = v[i];
        for (auto i = sz - 1; 1 <= i; i--) segs[i] = M::merge(segs[2 * i].m, segs[2 * i + 1].m);
    }

    size_type size() const {
        return segs.size() / 2;
    }

    template <typename T>
    void update_query(size_type ql, size_type qr, const T &t) {
        Op op(t);
        auto l0 = get_endpoint_seg(ql);
        auto r0 = get_endpoint_seg(qr);
        propagate_from_top(l0);
        propagate_from_top(r0);
        size_type lnode = ql + size(), rnode = qr + size();
        while (lnode < rnode) {
            if (lnode & 1) {
                segs[lnode].update_op(op);
                push(lnode);
                lnode++;
            }
            if (rnode & 1) {
                rnode--;
                segs[rnode].update_op(op);
                push(rnode);
            }
            lnode /= 2;
            rnode /= 2;
        }
        update_from_bottom(l0);
        update_from_bottom(r0);
    }

    M get_query(ll ql, ll qr) {
        auto ret = M::id_ele();
        auto l0 = get_endpoint_seg(ql);
        auto r0 = get_endpoint_seg(qr);
        propagate_from_top(l0);
        propagate_from_top(r0);
        size_type lnode = ql + size(), rnode = qr + size();
        while (lnode < rnode) {
            if (lnode & 1) {
                push(lnode);
                ret = M::merge(segs[lnode].m, ret);
                lnode++;
            }
            if (rnode & 1) {
                rnode--;
                push(rnode);
                ret = M::merge(ret, segs[rnode].m);
            }
            lnode /= 2;
            rnode /= 2;
        }
        return ret;
    }
};

}
