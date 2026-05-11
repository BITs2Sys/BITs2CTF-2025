#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;
using ll = long long;
using ld = long double;

ld getx(ll x0, ll x1, ll x2, ll v0, ll v1, ll v2, ld t) {
  bool is_mirrored = false;
  // Make v0 non-negative by mirroring the system by x=0
  // Before: x1(v1) |  x0(+v0) |  x2(v2)
  // After: -x2(v2) | -x0(-v0) | -x1(v1)
  if (v0 < 0) {
    x0 = -x0, x1 = -x1, x2 = -x2;
    v0 = -v0;
    swap(v1, v2);
    swap(x1, x2);
    is_mirrored = true;
  }

  auto calculate = [&]() -> ld {
    // In C++, if you don't like __float128, you can implement fractional
    // arithmetic instead, but __float128 is more convenient here.
    //
    // In Python, you can set the decimal precision to 30+ digits like this:
    //   import decimal
    //   decimal.getcontext().prec = 30
    using f128 = __float128;

    // Type 1: No collision at all
    if (v0 <= v2) return x0 + v0 * t;

    const f128 t1 = f128(x2 - x0) / (v0 - v2);

    // Type 2: The first collision is too late to happen
    if (t1 >= t) return x0 + v0 * t;
    // Type 3: Only one collision happens, the orb never reaches left wall
    if (v0 <= v1) return x2 + v2 * t1 - v0 * (t - t1);

    // Mysterious math happens here!
    const f128 c1 = f128(x2 - x1) / (v0 - v1);
    const f128 c2 = f128(x2 - x1) / (v0 - v2);
    const f128 k1 = f128(v0 + v2) / (v0 - v1);
    const f128 k2 = f128(v0 + v1) / (v0 - v2);
    const f128 A = k1 * k2;
    const f128 B = c2 + c1 * k2;
    const f128 P = t1 + B / (A - 1);
    const f128 Q = B / (A - 1);

    // Check if the sequence is linear (A = 1), iff v0 * (v1 + v2) = 0
    const bool linear = v0 == 0 || (v1 + v2) == 0;

    // The standard library doesn't have a power function for __float128
    // so we implement our own here
    auto pow = [](f128 a, ll b) -> f128 {
      f128 res = 1;
      while (b) {
        if (b & 1) res *= a;
        a *= a;
        b >>= 1;
      }
      return res;
    };

    // This function calculates T(n): time at the n-th collision
    auto getT = [&](ll n) -> f128 {
      ll half_n = (n - 1) >> 1;
      f128 tick = linear ? B * half_n + t1 : pow(A, half_n) * P - Q;
      return (n & 1) ? tick : c1 + k1 * tick;
    };

    // Use binary search to find the largest n such that T(n) < t, you can
    // use [1, 1e18] to pass this problem, but here is the fastest method:
    //
    // Approximate the number of collisions before time t.
    ll guess;
    if (linear) {
      guess = (t - t1) / B * 2 + 1;
    } else {
      guess = logl((t + Q) / P) / logl(A) * 2 + 1;
    }

    ll l = max(1ll, guess - 15), r = guess + 15, ans;
    f128 tans = 0;

    while (l <= r) {
      const ll mid = (l + r) >> 1;
      const f128 tmid = getT(mid);
      if (tmid < t) {
        l = mid + 1, ans = mid, tans = tmid;
      } else {
        r = mid - 1;
      }
    }

    // Calculate the final position after ans collisions
    // If ans is odd, the orb is moving right to left
    // If ans is even, the orb is moving left to right
    if (ans & 1) {
      return x2 + v2 * tans - v0 * (t - tans);
    } else {
      return x1 - v1 * tans + v0 * (t - tans);
    }
  };

  // Get the result, considering mirroring back
  ld result = calculate();
  return is_mirrored ? -result : result;
}

void solve() {
  ld rx, ry, rz, vx, vy, vz;
  ld x1, x2, y1, y2, z1, z2;
  ld vx1, vx2, vy1, vy2, vz1, vz2;

  cin >> rx >> ry >> rz >> vx >> vy >> vz;
  cin >> x1 >> x2 >> y1 >> y2 >> z1 >> z2;
  cin >> vx1 >> vx2 >> vy1 >> vy2 >> vz1 >> vz2;

  // Calculate the time when the spell card ends
  const ld t = (z2 - z1) / (vz1 + vz2);

  // For X and Y axis, calculate the final position after time t
  // For Z axis, it's just the final wall position
  cout << getx(rx, x1, x2, vx, vx1, vx2, t) << " ";
  cout << getx(ry, y1, y2, vy, vy1, vy2, t) << " ";
  cout << z1 + vz1 * t << "\n";
}

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  cout << fixed << setprecision(12);  // Don't forget to set precision!

  int T;
  cin >> T;
  while (T--) solve();
}
