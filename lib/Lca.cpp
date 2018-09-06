#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;
const ll size = 30;
const ll nptr = -1;

class Lca{
    private:
        ll N;
        ll root;
        vector<ll> depth;
        vector<vector<ll>> edge;
        vector<array<ll, size>> parents;

        void dfs(ll now, ll pre, ll d){
            parents[now][0] = nptr;
            depth[now] = d;
            for(ll next : edge[now]){
                if(next != pre){
                    dfs(next, now, d + 1);
                }
            }
        }

    public:
        Lca(ll n, ll r, const vector<vector<ll>> &e) : N(n), root(r), edge(e) {
            parents.resize(N);
            for(ll i = 0; i < N; i++){
                for(ll j = 0; j < size; j++){
                    parents[i][j] = nptr;
                }
            }
            
            dfs(root, nptr, 0);

            for(ll i = 1; i < size; i++){
                for(ll node = 0; node < N; node++){
                    if(parents[node][i - 1] == nptr){
                        parents[node][i] = nptr;
                    }else{
                        parents[node][i] = parents[parents[node][i - 1]][i - 1];
                    }
                }
            }
        }
        
        Lca(ll n, const vector<vector<ll>> &e){
            Lca(n, 0, e);
        }

        ll get_depth(ll node){
            return depth[node];
        }

        ll get_parents(ll node, ll relative_depth){
            ll ret = node;
            for(ll i = 0; (1 << i) <= relative_depth; i++){
                if(relative_depth & (1 << i)){
                    ret = parents[ret][i];
                }
            }
            return ret;
        }
        
        ll get_lca(ll n1, ll n2){
            ll d = min(depth[n1], depth[n2]);
            ll ok = 0, ng = d + 1;
            ll ret = nptr;
            while(abs(ok - ng) > 1){
                ll mid = (ok + ng) / 2;
                ll p1 = get_parents(n1, d - depth[n1]);
                ll p2 = get_parents(n2, d - depth[n2]);
                if(p1 == p2){
                    ok = mid;
                    ret = p1;
                }else{
                    ng = mid;
                }
            }
            return ret;
        }
};
