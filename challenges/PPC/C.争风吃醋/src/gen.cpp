#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <vector>
#define f(i, a, b) for (ll i = (a), _N_##i = (b); i <= _N_##i; i++)
#define fr(i, a, b) for (ll i = (a), _N_##i = (b); i >= _N_##i; i--)
#define Max(a, b) a = max((a), (b))
#define Min(a, b) a = min((a), (b))
#define endl '\n'
using namespace std;
using ll = long long;
using pii = pair<ll, ll>;

mt19937 rng(random_device{}());
ll rand(ll n) { return uniform_int_distribution<ll>(0, n - 1)(rng); }
ll rand(ll l, ll r) { return uniform_int_distribution<ll>(l, r)(rng); }
ll rand(pair<ll, ll> range) { return rand(range.first, range.second); }

const ll MAXN = 1000;

ll id = 0;

struct Graph {
  ll n, m, s, t;
  vector<pii> edges;
  vector<ll> adj[MAXN + 5];

  void clear() {
    edges.clear();
    f(i, 1, n) adj[i].clear();
  }

  void add_edge(ll u, ll v) {
    if (u > v) swap(u, v);
    edges.emplace_back(u, v);
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
};

// Generate a tree
Graph gen_tree(ll n) {
  Graph g;
  g.n = n;
  g.m = n - 1;
  g.s = rand(1, n);
  g.t = rand(1, n);
  while (g.t == g.s) g.t = rand(1, n);

  vector<ll> perm;
  f(i, 1, n) perm.push_back(i);
  shuffle(perm.begin(), perm.end(), rng);

  f(i, 1, n - 1) {
    ll u = perm[i];
    ll v = perm[rand(0, i - 1)];
    g.add_edge(u, v);
  }

  return g;
}

// Generate a cycle
Graph gen_cycle(ll n) {
  Graph g;
  g.n = n;
  g.m = n;

  vector<ll> perm;
  f(i, 1, n) perm.push_back(i);
  shuffle(perm.begin(), perm.end(), rng);

  f(i, 0, n - 1) { g.add_edge(perm[i], perm[(i + 1) % n]); }

  g.s = perm[0];
  g.t = perm[n / 2];

  return g;
}

// Generate a complete graph
Graph gen_complete(ll n) {
  Graph g;
  g.n = n;
  g.s = rand(1, n);
  g.t = rand(1, n);
  while (g.t == g.s) g.t = rand(1, n);

  f(i, 1, n) f(j, i + 1, n) g.add_edge(i, j);
  g.m = g.edges.size();

  return g;
}

// Generate a random connected graph
Graph gen_random(ll n, ll m) {
  Graph g;
  g.n = n;
  g.m = m;

  // First generate a tree to ensure connectivity
  vector<ll> perm;
  f(i, 1, n) perm.push_back(i);
  shuffle(perm.begin(), perm.end(), rng);

  f(i, 1, n - 1) {
    ll u = perm[i];
    ll v = perm[rand(0, i - 1)];
    g.add_edge(u, v);
  }

  // Add random edges
  set<pii> edge_set(g.edges.begin(), g.edges.end());
  while ((ll)g.edges.size() < m) {
    ll u = rand(1, n);
    ll v = rand(1, n);
    if (u == v) continue;
    if (u > v) swap(u, v);
    if (edge_set.count({u, v})) continue;

    g.add_edge(u, v);
    edge_set.insert({u, v});
  }

  g.s = rand(1, n);
  g.t = rand(1, n);
  while (g.t == g.s) g.t = rand(1, n);

  return g;
}

// Generate a graph where s-t is a bridge
Graph gen_bridge_st(ll n) {
  Graph g;
  g.n = n;

  if (n < 4) {
    // For small n, just generate a random graph
    return gen_random(n, rand(n - 1, n * (n - 1) / 2));
  }

  g.s = 1;
  g.t = n;

  // Split into two parts: [1, n1] and [n1+1, n]
  // We need at least 1 node on each side
  ll n1 = rand(max(1LL, n / 3), (2 * n) / 3);
  if (n1 < 1) n1 = 1;
  if (n - n1 < 1) n1 = n - 1;

  // Build structure on left side (containing s=1)
  if (n1 >= 3) {
    // Build a cycle: 1-2-3-...-n1-1
    f(i, 1, n1 - 1) g.add_edge(i, i + 1);
    g.add_edge(n1, 1);
  } else if (n1 == 2) {
    // Connect 1-2
    g.add_edge(1, 2);
  }
  // If n1 == 1, just node 1, no edges

  // Build structure on right side (containing t=n)
  if (n - n1 >= 3) {
    // Build a cycle: (n1+1)-(n1+2)-...-(n)-(n1+1)
    f(i, n1 + 1, n - 1) g.add_edge(i, i + 1);
    g.add_edge(n, n1 + 1);
  } else if (n - n1 == 2) {
    // Connect (n-1)-n
    g.add_edge(n - 1, n);
  }
  // If n - n1 == 1, just node n, no edges

  // Connect the two parts with a bridge edge
  // Must connect node 1 to node n to ensure s and t are connected
  g.add_edge(1, n);

  g.m = g.edges.size();
  return g;
}

void gen_case(ofstream& in_file, const Graph& g) {
  in_file << g.n << " " << g.m << " " << g.s << " " << g.t << endl;
  for (auto [u, v] : g.edges) {
    in_file << u << " " << v << endl;
  }
}

void gen_file(const vector<Graph>& graphs) {
  cout << "Generating file " << (id + 1) << " with " << graphs.size()
       << " test cases." << endl;
  cout.flush();

  char filename[20];
  sprintf(filename, "samples/%03lld.in", ++id);
  ofstream in_file(filename);

  in_file << graphs.size() << endl;
  for (auto& g : graphs) {
    gen_case(in_file, g);
  }
}

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);

  // Sample cases
  {
    Graph g;
    g.n = 2;
    g.m = 1;
    g.s = 1;
    g.t = 2;
    g.add_edge(1, 2);
    gen_file({g});
  }

  {
    Graph g;
    g.n = 3;
    g.m = 3;
    g.s = 1;
    g.t = 2;
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(2, 3);
    gen_file({g});
  }

  // Small trees (should be NO if s-t adjacent)
  {
    vector<Graph> cases;
    f(j, 1, 20) { cases.push_back(gen_tree(rand(3, 10))); }
    gen_file(cases);
  }

  // Small cycles (should be YES)
  {
    vector<Graph> cases;
    f(j, 1, 20) { cases.push_back(gen_cycle(rand(3, 15))); }
    gen_file(cases);
  }

  // Complete graphs (always YES)
  {
    vector<Graph> cases;
    f(j, 2, 20) { cases.push_back(gen_complete(j)); }
    gen_file(cases);
  }

  // Random small graphs
  {
    vector<Graph> cases;
    f(j, 1, 1000) {
      ll n = rand(2, 20);
      ll m = rand(n - 1, min(n * (n - 1) / 2, n * 2));
      cases.push_back(gen_random(n, m));
    }
    gen_file(cases);
  }

  // Medium graphs
  {
    vector<Graph> cases;
    f(j, 1, 1000) {
      ll n = rand(20, 100);
      ll m = rand(n - 1, min(n * (n - 1) / 2, n * 3));
      cases.push_back(gen_random(n, m));
    }
    gen_file(cases);
  }

  // Large n=100 graphs with various densities
  {
    vector<Graph> cases;
    f(j, 1, 1000) {
      ll n = 100;
      ll m = rand(n - 1, min(n * (n - 1) / 2, n * 4));
      cases.push_back(gen_random(n, m));
    }
    gen_file(cases);
  }

  // Very dense n=50 graphs
  {
    vector<Graph> cases;
    f(j, 1, 1000) {
      ll n = 50;
      ll m = rand(n * 5, min(n * (n - 1) / 2, 500ll));
      cases.push_back(gen_random(n, m));
    }
    gen_file(cases);
  }

  // Maximum density n=100 graphs (close to complete)
  {
    vector<Graph> cases;
    f(j, 1, 10) {
      ll n = 800;
      ll max_m = 2e4;
      ll m = rand(max_m - 50, max_m);
      cases.push_back(gen_random(n, m));
    }
    gen_file(cases);
  }

  // Bridge cases (mix of YES and NO)
  {
    vector<Graph> cases;
    f(j, 1, 100) { cases.push_back(gen_bridge_st(rand(4, 1000))); }
    gen_file(cases);
  }

  // Sparse graph
  {
    vector<Graph> cases;
    f(j, 1, 200) {
      ll n = rand(10, 1000);
      ll m = rand(n - 1, n + 10);
      cases.push_back(gen_random(n, m));
    }
    gen_file(cases);
  }

  // Trees of various sizes
  {
    vector<Graph> cases;
    f(j, 1, 500) {
      ll n = rand(10, 1000);
      cases.push_back(gen_tree(n));
    }
    gen_file(cases);
  }

  // Cycles of various sizes
  {
    vector<Graph> cases;
    f(j, 1, 500) {
      ll n = rand(10, 1000);
      cases.push_back(gen_cycle(n));
    }
    gen_file(cases);
  }

  // Complete graphs of various sizes
  {
    vector<Graph> cases;
    f(j, 2, 100) {
      ll n = j;
      cases.push_back(gen_complete(n));
    }
    gen_file(cases);
  }

  {
    vector<Graph> cases;
    ll n = 1000;
    cases.push_back(gen_complete(n));
    gen_file(cases);
  }

  // High T (many small test cases)
  {
    vector<Graph> cases;
    f(j, 1, 1000) {
      ll n = rand(2, 10);
      ll m = rand(n - 1, min(n * (n - 1) / 2, n + 5));
      cases.push_back(gen_random(n, m));
    }
    gen_file(cases);
  }

  // Random mixed cases
  {
    vector<Graph> cases;
    ll num = 1000;
    ll sum_m = 0;
    f(j, 1, num) {
      ll n = rand(2, 50);
      ll m = rand(n - 1, min(n * (n - 1) / 2, n * 3));
      sum_m += m;
      if (sum_m >= 5e5) break;
      cases.push_back(gen_random(n, m));
    }
    gen_file(cases);
  }

  cout << "Generated " << id << " test files." << endl;
}
