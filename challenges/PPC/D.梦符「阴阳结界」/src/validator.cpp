#include "../testlib.h"
using namespace std;

const long long MIN_COORD = -1e9;
const long long MAX_COORD = 1e9;

const long long MIN_V = 0;
const long long MAX_V = 1e9;

int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  int T = inf.readInt(1, 10000, "T");
  inf.readEoln();

  for (int i = 0; i < T; i++) {
    setTestCase(i + 1);

    // Line 1: rx, ry, rz, vx, vy, vz
    long long rx = inf.readLong(MIN_COORD, MAX_COORD, "rx");
    inf.readSpace();
    long long ry = inf.readLong(MIN_COORD, MAX_COORD, "ry");
    inf.readSpace();
    long long rz = inf.readLong(MIN_COORD, MAX_COORD, "rz");
    inf.readSpace();
    inf.readLong(MIN_COORD, MAX_COORD, "vx");
    inf.readSpace();
    inf.readLong(MIN_COORD, MAX_COORD, "vy");
    inf.readSpace();
    inf.readLong(MIN_COORD, MAX_COORD, "vz");
    inf.readEoln();

    // Line 2: x1, x2, y1, y2, z1, z2
    long long x1 = inf.readLong(MIN_COORD, MAX_COORD, "x1");
    inf.readSpace();
    long long x2 = inf.readLong(MIN_COORD, MAX_COORD, "x2");
    inf.readSpace();
    long long y1 = inf.readLong(MIN_COORD, MAX_COORD, "y1");
    inf.readSpace();
    long long y2 = inf.readLong(MIN_COORD, MAX_COORD, "y2");
    inf.readSpace();
    long long z1 = inf.readLong(MIN_COORD, MAX_COORD, "z1");
    inf.readSpace();
    long long z2 = inf.readLong(MIN_COORD, MAX_COORD, "z2");
    inf.readEoln();

    // Line 3: vx1, vx2, vy1, vy2, vz1, vz2
    inf.readLong(MIN_V, MAX_V, "vx1");
    inf.readSpace();
    inf.readLong(MIN_V, MAX_V, "vx2");
    inf.readSpace();
    inf.readLong(MIN_V, MAX_V, "vy1");
    inf.readSpace();
    inf.readLong(MIN_V, MAX_V, "vy2");
    inf.readSpace();
    long long vz1 = inf.readLong(MIN_V, MAX_V, "vz1");
    inf.readSpace();
    long long vz2 = inf.readLong(MIN_V, MAX_V, "vz2");
    inf.readEoln();

    // Constraints check
    ensuref(vz1 + vz2 > 0, "Sum of vz1 and vz2 must be positive");

    ensuref(x1 < rx && rx < x2,
            "Orb must be strictly inside x boundaries: %lld < %lld < %lld", x1,
            rx, x2);
    ensuref(y1 < ry && ry < y2,
            "Orb must be strictly inside y boundaries: %lld < %lld < %lld", y1,
            ry, y2);
    ensuref(z1 < rz && rz < z2,
            "Orb must be strictly inside z boundaries: %lld < %lld < %lld", z1,
            rz, z2);
  }

  inf.readEof();
  return 0;
}
