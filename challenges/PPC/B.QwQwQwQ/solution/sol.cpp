#include <iostream>
#include <limits>
#include <string>
#include <vector>
#define endl '\n'
using namespace std;
using ll = long long;

ll q, k, x;
string str;

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);

  cin >> str >> q;
  const int n = str.size();

  vector<ll> pre(n + 1);

  // pre[i]: number of 'q' in str[1..i]
  for (int i = 1; i <= n; i++) pre[i] = pre[i - 1] + (str[i - 1] == 'q');

  // Calculate the length of string s[1..i] after k expansions
  auto calc = [&pre](ll k, ll i) -> ll {
    if (pre[i] == 0) return i;
    if (k > 30) return numeric_limits<ll>::max();
    return pre[i] * ((1ll << (k + 1)) - 2) + i;
  };

  while (q--) {
    cin >> k >> x;

    // NOTE: The difficulty of this problem has been reduced, you can
    // just enumerate the index, but here is the binary search solution.
    //
    // Binary search for the smallest index with calc(k, index) >= x
    ll l = 0, r = n, ans = -1;
    while (l <= r) {
      ll mid = (l + r) >> 1;
      if (calc(k, mid) >= x)
        r = mid - 1, ans = mid;
      else
        l = mid + 1;
    }

    if (ans == -1) {
      cout << '0';
    } else if (str[ans - 1] == 'q') {
      // q -> qwq -> qwqwqwq ...
      // Therefore 'q' or 'w' only depends on the index
      cout << ((x - calc(k, ans - 1)) % 2 == 1 ? 'q' : 'w');
    } else {
      // Don't forget std::string is 0-indexed!
      cout << str[ans - 1];
    }
  }
  cout << endl;
}
