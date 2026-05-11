#include <algorithm>
#include <numeric>
#include <set>
#include <vector>

#include "../testlib.h"

using namespace std;

struct DSU {
  vector<int> fa;
  int count;
  DSU(int n) : fa(n + 1), count(n) { iota(fa.begin(), fa.end(), 0); }
  int find(int x) { return fa[x] == x ? x : fa[x] = find(fa[x]); }
  void merge(int x, int y) {
    x = find(x), y = find(y);
    if (x != y) fa[x] = y, count--;
  }
};

int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  int T = inf.readInt(1, 1000, "T");
  inf.readEoln();

  int sum = 0;

  for (int test_i = 0; test_i < T; ++test_i) {
    int n = inf.readInt(2, 1000, "n");
    inf.readSpace();
    int m = inf.readInt(n - 1, n * (n - 1) / 2, "m");
    sum += m;
    inf.readSpace();
    int s = inf.readInt(1, n, "s");
    inf.readSpace();
    int t = inf.readInt(1, n, "t");
    inf.readEoln();

    ensuref(sum <= 5e5,
            "(testcase %d) Sum of m over all test cases exceeds 500000",
            test_i);
    ensuref(s != t, "(testcase %d) s and t must be distinct", test_i);

    set<pair<int, int>> edges;
    DSU dsu(n);

    for (int i = 0; i < m; ++i) {
      int u = inf.readInt(1, n, "u");
      inf.readSpace();
      int v = inf.readInt(1, n, "v");
      inf.readEoln();

      ensuref(u != v, "(testcase %d) Self-loop detected: %d %d", test_i, u, v);

      if (u > v) swap(u, v);
      ensuref(edges.find({u, v}) == edges.end(),
              "(testcase %d) Duplicate edge detected: %d %d", test_i, u, v);
      edges.insert({u, v});

      dsu.merge(u, v);
    }

    ensuref(dsu.count == 1, "(testcase %d) Graph is not connected", test_i);
  }

  inf.readEof();
}
