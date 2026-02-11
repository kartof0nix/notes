#include <bits/stdc++.h>
using namespace std;

// constexpr int MAXN = 1<<3; // 1e6


typedef long long ll;
#define rep(i, a, b) for(int i = a; i < b; i++)

constexpr ll oo = 1e16;
 
struct prz_prz{
    const ll v0;
    static const int MAXN = 1<<19;
    ll (*mrg)(ll a, ll b);
    void (*app)(ll &elem, ll v);
    ll val[2*MAXN], psh[2*MAXN];
    void calc() {for(int i = MAXN-1; i >= 1; i--) val[i] = mrg(val[i*2], val[i*2+1]); }
    prz_prz(ll v0, ll (*merge)(ll a, ll b), void (*apply)(ll& ,ll)) : v0(v0), mrg(merge), app(apply) {
        rep(i, MAXN, 2*MAXN) val[i] = v0;
        calc();
    };
    void push(int i){
        app(val[i*2], psh[i]); app(psh[i*2], psh[i]);
        app(val[i*2 + 1], psh[i]); app(psh[i*2 + 1], psh[i]);
        psh[i] = 0;
    }
    void _mod(int i, int ip, int ik, const int p, const int k, ll v){
        if(p <= ip && ik <= k){
            app(val[i], v); app(psh[i], v);
            return;
        }
        if(k < ip || ik < p) return;
        push(i);
        _mod(i*2, ip, (ip+ik)/2, p, k, v);
        _mod(i*2+1, (ip+ik)/2+1, ik, p, k, v);
        val[i] = mrg(val[i*2], val[i*2+1]);
    }
    void mod(int p, int k, ll v){
        _mod(1, 0, MAXN-1, p, k, v);
    }
    ll _get(int i, int ip, int ik, const int p, const int k){
        if(p <= ip && ik <= k){
            return val[i];
        }
        if(k < ip || ik < p) return v0;
        push(i);
        ll r = _get(i*2, ip, (ip+ik)/2, p, k);
        r = mrg(r, _get(i*2+1, (ip+ik)/2+1, ik, p, k));
        val[i] = mrg(val[i*2], val[i*2+1]);
        return r;
    }
    ll get(int p, int k) { return _get(1, 0, MAXN-1, p, k); }
    ll& operator[](int i){ return val[i+MAXN];}
};

prz_prz drz{
    +oo, 
    [](ll a, ll b) { return min(a, b); },
    [](ll&elem, ll v) { elem += v;} 
};

int main(){
    int n, q;
    cin >> n >> q;
    for(int i = 0; i < n; i++){
        cin >> drz[i];
    }
    drz.calc();
    while(q--){
        int t; cin >> t;
        int a, b, v;
        switch(t){
            case 0:
                cin >> a >> b >> v;
                drz.mod(a, b-1, v);
                break;
            case 1:

            cin >> a >> b;
                cout << drz.get(a, b-1) << '\n';
                break;

        }
    }
}