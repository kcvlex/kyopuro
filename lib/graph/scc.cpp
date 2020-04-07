#include "../util/template.cpp"
#include "graph.cpp"

namespace graph {

template <typename Graph>
class StronglyConnectedComponents {
    const Graph &graph;
    Graph rgraph;
    vec<ll> label, scc_ord;

    void dfs1(ll cur, ll &l) {
        label[cur] = -2;
        for (const graph::Edge &e : graph[cur]) {
            ll nxt;
            std::tie(nxt, std::ignore) = e;
            if (label[nxt] != -1) continue;
            dfs1(nxt, l);
        }
        label[cur] = l++;
    }

    void write_label() {
        ll l = 0;
        for (ll i = 0; i < graph.size(); i++) if (label[i] == -1) dfs1(i, l);
    }

    void dfs2(ll cur, ll l, ll &idx, vec<ll> &result) {
        result[cur] = l;
        scc_ord[idx++] = cur;
        for (const graph::Edge &e : rgraph[cur]) {
            ll nxt;
            std::tie(nxt, std::ignore) = e;
            if (result[nxt] != -1) continue;
            dfs2(nxt, l, idx, result);
        }
    }

    vec<ll> build_scc() {
        ll l = 0;
        vec<ll> result(graph.size(), -1), ord(rgraph.size());
        std::iota(ALL(ord), 0ll);
        std::sort(ALL(ord), [&](ll i, ll j) { return label[i] > label[j]; });
        ll idx = 0;
        for (ll n : ord) if (result[n] == -1) dfs2(n, l++, idx, result);
        return result;
    }

public:
    StronglyConnectedComponents(const Graph &graph)
        : graph(graph), label(graph.size(), -1), scc_ord(graph.size())
    {
        rgraph = graph.build_inv();
    }

    vec<ll> build() {
        vec<ll> result(graph.size(), -1);
        write_label();
        return build_scc();
    }

    std::pair<::graph::Graph<true>, vec<ll>> build_scc_graph() {
        auto scc_label = build();
        ::graph::Graph<true> scc_graph(*max_element(ALL(scc_label)) + 1);
        for (ll from = 0; from < graph.size(); from++) for (auto &&e : graph[from]) {
            ll to;
            std::tie(to, std::ignore) = e;
            ll lf = scc_label[from], lt = scc_label[to];
            if (lf != lt) scc_graph.add_edge(lf, lt);
        }
        return std::make_pair(std::move(scc_graph), std::move(scc_label));
    }
};

template <typename Graph> using scc = StronglyConnectedComponents<Graph>;

}