#include <fstream>
#include <iostream>
#include <ostream>
#include <random>
#define f(i, a, b) for (ll i = (a), _N_##i = (b); i <= _N_##i; i++)
#define fr(i, a, b) for (ll i = (a), _N_##i = (b); i >= _N_##i; i--)
#define Max(a, b) a = max((a), (b))
#define Min(a, b) a = min((a), (b))
#define endl '\n'
using namespace std;
using ll = long long;
using ld = long double;

mt19937 rng(random_device{}());
ll rand(ll n) { return uniform_int_distribution<ll>(0, n - 1)(rng); }
ll rand(ll l, ll r) { return uniform_int_distribution<ll>(l, r)(rng); }
ll rand(pair<ll, ll> range) { return rand(range.first, range.second); }

ll id = 0;

void gen(vector<ll> v) {
  char filename[20];
  sprintf(filename, "samples/%03lld.in", ++id);
  ofstream in(filename);

  const ll n = v.size();
  in << n << endl;
  f(i, 0, n - 1) in << v[i] << " \n"[i == n - 1];
}

void gen_by_range(pair<ll, ll> nrange, pair<ll, ll> vrange) {
  ll n = rand(nrange);
  vector<ll> v;
  v.reserve(n);
  f(i, 1, n) v.push_back(rand(vrange));
  gen(v);
}

const ll MAXN = 1e5;
const ll MAXV = 1e9;

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);

  gen({1});
  gen({5, 1, 4, 1});

  gen_by_range({100, 100}, {1, 1});
  gen_by_range({100, 100}, {1, 3});
  gen_by_range({100, 100}, {1, 10});
  gen_by_range({1e3, 1e3}, {10, 100});
  gen_by_range({1e3, 1e3}, {100, 1000});
  gen_by_range({1e4, 1e4}, {1, 10000});
  gen_by_range({MAXN, MAXN}, {1, 100000});
  gen_by_range({MAXN, MAXN}, {1000, MAXV});
  gen_by_range({MAXN, MAXN}, {1e7, MAXV});
  gen_by_range({MAXN, MAXN}, {MAXV, MAXV});

  gen_by_range({1, MAXN}, {1, MAXV});
  gen_by_range({MAXN, MAXN}, {1e7, MAXV});
  gen_by_range({1e4, MAXN}, {1, MAXV});
}
