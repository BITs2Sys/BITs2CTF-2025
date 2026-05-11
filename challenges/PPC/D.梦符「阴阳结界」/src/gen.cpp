#include <cassert>
#include <fstream>
#include <iostream>
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

struct CaseParams {
  pair<ll, ll> coord_range;     // Range for coordinates center
  pair<ll, ll> width_range;     // Range for width (x2-x1)
  pair<ll, ll> v_orb_range;     // Range for orb velocity magnitude
  pair<ll, ll> v_wall_range;    // Range for wall velocity (X/Y)
  pair<ll, ll> z_height_range;  // Range for Z height
  pair<ll, ll> v_z_range;       // Range for Z velocity

  bool zero_orb_v_xy = false;
  bool zero_wall_v_xy = false;
  bool start_near_wall = false;
  bool narrow_box = false;

  // New flags
  bool wall_outrun_orb = false;  // Wall expansion speed > Orb speed
  bool one_sided_z = false;      // One of vz1 or vz2 is 0
  bool tiny_z_height = false;    // z2 - z1 is very small
  bool planar_xz = false;        // Motion only in XZ plane (vy=0, vy1=vy2=0)
};

void gen_case(ofstream& in, CaseParams p) {
  auto gen_interval = [&](ll& p1, ll& p2, bool is_z) {
    ll w;
    if (is_z) {
      if (p.tiny_z_height)
        w = rand(2, 100);
      else
        w = rand(p.z_height_range);
    } else if (p.narrow_box)
      w = rand(2, 20);
    else
      w = rand(p.width_range);

    ll min_val = p.coord_range.first;
    ll max_val = p.coord_range.second;

    // Clamp width
    if (max_val - min_val < w) w = max_val - min_val;
    if (w < 2) w = 2;

    p1 = rand(min_val, max_val - w);
    p2 = p1 + w;
  };

  ll x1, x2, y1, y2, z1, z2;
  gen_interval(x1, x2, false);
  gen_interval(y1, y2, false);
  gen_interval(z1, z2, true);

  // Generate orb position
  ll rx, ry, rz;
  if (p.start_near_wall) {
    // 50% chance to be very close to a wall
    rx = (rand(2) ? x1 + 1 : x2 - 1);
    ry = (rand(2) ? y1 + 1 : y2 - 1);
    rz = (rand(2) ? z1 + 1 : z2 - 1);
    // Add some noise so it's not always exactly 1 unit away
    if (x2 - x1 > 10) rx = rand(x1 + 1, x1 + 1 + rand(5));
    if (y2 - y1 > 10) ry = rand(y1 + 1, y1 + 1 + rand(5));
  } else {
    rx = rand(x1 + 1, x2 - 1);
    ry = rand(y1 + 1, y2 - 1);
    rz = rand(z1 + 1, z2 - 1);
  }

  // Generate orb velocity
  ll vx, vy, vz;
  if (p.zero_orb_v_xy) {
    vx = 0;
    vy = 0;
  } else if (p.planar_xz) {
    vx = rand(p.v_orb_range) * (rand(2) ? 1 : -1);
    vy = 0;
  } else {
    vx = rand(p.v_orb_range) * (rand(2) ? 1 : -1);
    vy = rand(p.v_orb_range) * (rand(2) ? 1 : -1);
  }
  vz = rand(p.v_orb_range) * (rand(2) ? 1 : -1);

  // Generate wall velocities
  ll vx1, vx2, vy1, vy2;
  if (p.zero_wall_v_xy) {
    vx1 = vx2 = vy1 = vy2 = 0;
  } else if (p.planar_xz) {
    vx1 = rand(p.v_wall_range);
    vx2 = rand(p.v_wall_range);
    vy1 = vy2 = 0;
  } else if (p.wall_outrun_orb) {
    // Make wall expansion faster than orb speed
    // Ensure orb speed is relatively small
    if (abs(vx) > 0)
      vx1 = abs(vx) + rand(10, 1000);
    else
      vx1 = rand(p.v_wall_range);
    if (abs(vx) > 0)
      vx2 = abs(vx) + rand(10, 1000);
    else
      vx2 = rand(p.v_wall_range);
    if (abs(vy) > 0)
      vy1 = abs(vy) + rand(10, 1000);
    else
      vy1 = rand(p.v_wall_range);
    if (abs(vy) > 0)
      vy2 = abs(vy) + rand(10, 1000);
    else
      vy2 = rand(p.v_wall_range);
  } else {
    vx1 = rand(p.v_wall_range);
    vx2 = rand(p.v_wall_range);
    vy1 = rand(p.v_wall_range);
    vy2 = rand(p.v_wall_range);
  }

  ll vz1, vz2;
  if (p.one_sided_z) {
    if (rand(2)) {
      vz1 = 0;
      vz2 = rand(p.v_z_range);
      if (vz2 == 0) vz2 = 1;
    } else {
      vz2 = 0;
      vz1 = rand(p.v_z_range);
      if (vz1 == 0) vz1 = 1;
    }
  } else {
    vz1 = rand(p.v_z_range);
    vz2 = rand(p.v_z_range);
    // Ensure z collapses
    if (vz1 + vz2 == 0) vz1 = 1;
  }

  in << rx << " " << ry << " " << rz << " " << vx << " " << vy << " " << vz
     << endl;
  in << x1 << " " << x2 << " " << y1 << " " << y2 << " " << z1 << " " << z2
     << endl;
  in << vx1 << " " << vx2 << " " << vy1 << " " << vy2 << " " << vz1 << " "
     << vz2 << endl;
}

void gen(int count, int T, CaseParams p) {
  while (count--) {
    char filename[20];
    sprintf(filename, "samples/%03lld.in", ++id);
    ofstream in(filename);
    in << T << endl;
    f(i, 1, T) gen_case(in, p);
  }
}

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  id = 0;

  // Example 1
  {
    char filename[20];
    sprintf(filename, "samples/%03lld.in", ++id);
    ofstream in(filename);
    in << "2" << endl;
    in << "5 5 1 10 0 0" << endl;
    in << "0 10 0 10 0 2" << endl;
    in << "0 0 0 0 1 1" << endl;
    in << "10 5 2 -10 0 0" << endl;
    in << "0 20 0 10 0 4" << endl;
    in << "1 1 0 0 1 1" << endl;
  }

  // Example 2
  {
    char filename[20];
    sprintf(filename, "samples/%03lld.in", ++id);
    ofstream in(filename);
    in << "1" << endl;
    in << "0 0 0 1000000000 0 0" << endl;
    in << "-1 1 -1 1 -1000000000 1000000000" << endl;
    in << "0 0 0 0 1 0" << endl;
  }

  // Small Random (Sanity)
  gen(1, 1, {{-10, 10}, {10, 30}, {0, 10}, {0, 5}, {10, 30}, {0, 5}});

  // Large Random (General)
  gen(1, 5,
      {{-1e8, 1e8}, {1e7, 1e8}, {0, 1e8}, {0, 1e7}, {1e7, 1e8}, {0, 1e7}});

  // High Velocity Orb (Many collisions)
  gen(1, 100,
      {{-10000, 10000},
       {100, 1000},
       {1e7, 1e8},
       {0, 10},
       {1000, 5000},
       {0, 10}});

  // High Velocity Walls (Fast shrinking)
  gen(1, 1000,
      {{-1e8, 1e8},
       {1e7, 1e8},
       {1e3, 1e4},
       {1e6, 1e7},
       {1e7, 1e8},
       {1e6, 1e7}});

  // Wall Outruns Orb (No XY collision)
  gen(1, 10000,
      {
          {-1e8, 1e8},
          {1e7, 1e8},
          {1e3, 1e4},
          {1e6, 1e7},
          {1e7, 1e8},
          {1e6, 1e7},
          false,
          false,
          false,
          false,
          true  // wall_outrun_orb
      });

  // Long Duration (Slow Z shrink)
  gen(1, 10000,
      {{-1e8, 1e8}, {1e7, 1e8}, {1e3, 1e4}, {0, 100}, {1e7, 1e8}, {0, 5}});

  // Short Duration (Fast Z shrink)
  gen(1, 10000,
      {{-1e8, 1e8},
       {1e7, 1e8},
       {1e3, 1e4},
       {0, 100},
       {100, 1000},
       {100, 1000}});

  // Tiny Z Height (Start near end)
  gen(1, 10000,
      {
          {-1e8, 1e8},
          {1e7, 1e8},
          {1e3, 1e4},
          {0, 100},
          {100, 1000},
          {100, 1000},
          false,
          false,
          false,
          false,
          false,
          false,
          true  // tiny_z_height
      });

  // Narrow Box
  gen(1, 10000,
      {
          {-1e4, 1e4},
          {0, 0},
          {1e3, 1e4},
          {0, 10},
          {1e3, 1e4},
          {10, 100},
          false,
          false,
          false,
          true  // narrow_box = true
      });

  // Pure Z Motion (1D)
  gen(1, 10000,
      {
          {-1e8, 1e8},
          {1e7, 1e8},
          {1e3, 1e4},
          {0, 0},
          {1e7, 1e8},
          {10, 100},
          true,
          true  // zero_orb_v_xy, zero_wall_v_xy
      });

  // Planar Motion (XZ)
  gen(1, 10000,
      {
          {-1e8, 1e8},
          {1e7, 1e8},
          {1e3, 1e4},
          {0, 0},
          {1e7, 1e8},
          {10, 100},
          false,
          false,
          false,
          false,
          false,
          false,
          false,
          true  // planar_xz
      });

  // Boundary Start
  gen(1, 10000,
      {
          {-1e8, 1e8},
          {1e7, 1e8},
          {1e4, 1e5},
          {0, 100},
          {1e7, 1e8},
          {10, 100},
          false,
          false,
          true  // start_near_wall
      });

  // One-sided Z Collapse
  gen(1, 10000,
      {
          {-1e8, 1e8},
          {1e7, 1e8},
          {1e4, 1e5},
          {0, 100},
          {1e7, 1e8},
          {10, 100},
          false,
          false,
          false,
          false,
          false,
          true  // one_sided_z
      });

  // Max Constraints
  gen(1, 10000,
      {{-1e8, 1e8},
       {1e8, 1e8},
       {1e8, 1e8},
       {1e8, 1e8},
       {1e8, 1e8},
       {1e8, 1e8}});
}
