// Just a copy from std.cpp but output "YES" or "NO" only

#include <iostream>
#include <queue>
#include <vector>
#define endl '\n'
using namespace std;
const int N = 1000 + 7;

int T, n, m, s, t;
bool ed[N][N];
vector<int> edges[N];

// Restrict street direction: u -> v
void set_edge(int u, int v) {
  ed[u][v] = true;
  ed[v][u] = false;
}

// Make crossroad u blocked
void block_node(int u) {
  for (int v : edges[u]) set_edge(v, u);
}

void output() {
  cout << "YES" << endl;
  // for (int u = 1; u <= n; u++)
  //   for (int v = 1; v <= n; v++)
  //     if (ed[u][v]) {
  //       cout << u << " " << v << endl;
  //       ed[v][u] = false;  // If not set_edge, avoid duplicate output
  //     }
}

bool bfs1() {
  queue<int> q;
  vector<bool> vis(n + 1, false);
  vector<int> fa(n + 1);

  q.emplace(s);
  vis[s] = true;
  fa[s] = t;
  while (!q.empty()) {
    int u = q.front();
    q.pop();

    if (u == t) {
      // Step 1: Block all nodes in the cycle
      for (int cur = t; cur != s; cur = fa[cur]) block_node(cur);
      block_node(s);

      // Step 2: Make a cycle s -> ... -> u -> t -> s
      set_edge(s, t);
      for (int cur = t; cur != s; cur = fa[cur]) set_edge(cur, fa[cur]);

      output();
      return true;
    }

    for (int v : edges[u]) {
      // s <-> t edge or visited, continue
      if (!ed[u][v] || vis[v]) continue;

      vis[v] = true;
      fa[v] = u;
      q.emplace(v);
    }
  }

  return false;
}

bool bfs2() {
  queue<int> q;
  vector<bool> vis(n + 1, false);
  vector<int> fa(n + 1);

  q.emplace(s);
  vis[s] = true;
  while (!q.empty()) {
    int u = q.front();
    q.pop();

    for (int v : edges[u]) {
      // Road visited, continue
      if (!ed[u][v]) continue;

      if (vis[v]) {
        // Step 1: Block all nodes in the cycle and tail
        for (int cur = u; cur != s; cur = fa[cur]) block_node(cur);
        for (int cur = v; cur != s; cur = fa[cur]) block_node(cur);
        block_node(s);
        block_node(t);

        // Step 2: Make cycle and tail
        set_edge(t, s);
        set_edge(v, u);
        for (int cur = u; cur != s; cur = fa[cur]) set_edge(cur, fa[cur]);
        for (int cur = v; cur != s; cur = fa[cur]) set_edge(fa[cur], cur);

        output();
        return true;
      }

      vis[v] = true;
      fa[v] = u;
      set_edge(u, v);  // Mark road as visited
      q.emplace(v);
    }
  }

  return false;
}

void solve() {
  cin >> n >> m >> s >> t;

  // Initialize
  for (int i = 1; i <= n; i++) {
    edges[i].clear();
    for (int j = 1; j <= n; j++) ed[i][j] = false;
  }

  // Read edges
  for (int i = 1; i <= m; i++) {
    int u, v;
    cin >> u >> v;
    ed[u][v] = ed[v][u] = true;
    edges[u].push_back(v);
    edges[v].push_back(u);
  }

  // Type 1: s and t are not directly connected, just block them both
  if (!ed[s][t]) {
    block_node(s);
    block_node(t);
    output();
    return;
  }

  // Type 2: s <-> t edge is not a bridge (SCC, can find a cycle)
  // Just break the edge and use BFS to find a cycle
  ed[s][t] = ed[t][s] = false;
  if (bfs1()) return;

  // Type 3: s <-> t edge is a bridge, need to find a cycle
  // and a tail connecting to s and t respectively
  ed[s][t] = ed[t][s] = true;
  if (bfs2()) return;

  // Type 4: No solution, the whole graph is a tree (no cycle at all)
  cout << "NO" << endl;
}

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);

  cin >> T;
  while (T--) solve();
}
