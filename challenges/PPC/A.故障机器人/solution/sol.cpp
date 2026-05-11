#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

ll n;

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);

  cin >> n;

  vector<ll> a(n + 1);

  for (ll i = 1; i <= n; i++) cin >> a[i];

  // Sort the birds by HP. The robot will focus on the weakest bird until it
  // dies before moving to the next one.
  sort(a.begin(), a.end());

  ll total_damage = 0;

  // Iterate through each bird in the order they will be killed
  for (ll i = 1; i <= n; i++) {
    ll hp = a[i];

    // Calculate rounds needed to kill the current bird.
    // Robot deals 3 damage per turn.
    // rounds = ceil(hp / 3.0) -> using integer arithmetic: (hp + 2) / 3
    ll rounds = (hp + 2) / 3;

    // For each round we have (n - i) birds attacking.
    // We kill the bird at the last round, so minus 1.
    total_damage += rounds * (n - i) - 1;
  }

  cout << total_damage;
}