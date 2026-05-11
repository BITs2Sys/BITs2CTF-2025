#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
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
ld randld(ld l, ld r) { return uniform_real_distribution<ld>(l, r)(rng); }

const ll MAXX = 1e9;

inline char rand_without_q() {
  char ch = rand('a', 'z' - 1);
  return ch == 'q' ? 'z' : ch;
}

string gen_string(ll type, ll len) {
  switch (type) {
    case 1:  // All Q
      return string(len, 'q');
    case 2:  // No Q
    {
      string s;
      f(i, 1, len) s += rand_without_q();
      return s;
    }
    case 3:  // Only one Q
    {
      string s;
      f(i, 1, len) s += rand_without_q();
      if (len > 0) s[rand(len)] = 'q';
      return s;
    }
    case 4:  // Q at the beginning
    {
      ll q_len = rand(1, max(2ll, len / 10));
      string s(q_len, 'q');
      f(i, 1, len - q_len) s += rand_without_q();
      return s;
    }
    case 5:  // Q at the end
    {
      ll q_len = rand(1, max(2ll, len / 10));
      string s;
      f(i, 1, len - q_len) s += rand_without_q();
      return s + string(q_len, 'q');
    }
    case 6:  // Q and W
    {
      string s;
      f(i, 1, len) s += rand(2) ? 'q' : 'w';
      return s;
    }
    default:  // Random Q
      ld q_prob = randld(0, 0.01);
      string s;
      f(i, 1, len) {
        if (randld(0, 1) < q_prob)
          s += 'q';
        else
          s += rand_without_q();
      }
      return s;
  }
}

// 计算字符串进行k次操作后的长度
ll calc(ll k, ll len, ll q_cnt) {
  if (q_cnt == 0) return len;
  if (k > 30) return 2e18;  // Sufficiently large
  // len + q_cnt * (2^(k+1) - 2)
  return q_cnt * ((1ll << (k + 1)) - 2) + len;
}

ll id = 0;

void gen(pair<ll, ll> srange, pair<ll, ll> qrange, pair<ll, ll> krange,
         ll stype) {
  char filename[20];
  sprintf(filename, "samples/%03lld.in", ++id);
  ofstream in(filename);

  ll len = rand(srange);
  string s = gen_string(stype, len);

  in << s << endl;

  ll q = rand(qrange);

  in << q << endl;

  ll q_cnt = 0;
  for (char c : s)
    if (c == 'q') q_cnt++;

  f(i, 1, q) {
    ll k = rand(krange);
    ll current_len = calc(k, s.size(), q_cnt);

    ll x = rand(1, MAXX);
    if (current_len < MAXX) {
      // 70% 概率生成合法位置, 20% 概率稍微越界, 10% 概率完全随机
      ll choice = rand(10);
      if (choice < 7) {
        x = rand(1, current_len);
      } else if (choice < 9) {
        x = rand(current_len + 1, min(MAXX, current_len + 10));
      }
    }

    in << k << ' ' << x << endl;
  }
}

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);

  id = 2;
  // s length up to 1000
  // q up to 10^5

  // Small cases
  f(i, 1, 10) gen({1, 10}, {1, 10}, {1, 5}, i);

  // Medium cases
  f(i, 1, 10) gen({10, 100}, {10, 100}, {1, 20}, i);

  // Max constraints - different string patterns
  f(i, 1, 10) gen({900, 1000}, {90000, 100000}, {1, 1e9}, i);

  // Specific patterns
  f(i, 1, 10) gen({1000, 1000}, {100000, 100000}, {1e8, 1e9}, i);
}
