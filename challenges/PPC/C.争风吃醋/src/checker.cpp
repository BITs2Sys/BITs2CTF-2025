#include <bitset>
#include <queue>

#include "../testlib.h"

using namespace std;

const int MAXN = 1000 + 7;

void solve() {
  int n = inf.readInt();
  int m = inf.readInt();
  int s = inf.readInt();
  int t = inf.readInt();

  set<pair<int, int>> input_edges;
  for (int i = 0; i < m; ++i) {
    int u = inf.readInt();
    int v = inf.readInt();
    if (u > v) swap(u, v);
    input_edges.insert({u, v});
  }

  string ja = ans.readToken();  // Jury Answer
  string pa = ouf.readToken();  // Participant Answer

  // 统一转大写
  for (auto& c : ja) c = toupper(c);
  for (auto& c : pa) c = toupper(c);

  // Step 1. 检查 YES/NO 的基本逻辑
  if (pa != "YES" && pa != "NO") {
    quitf(_pe, "Output must be YES or NO");
  }

  if (pa == "NO") {
    if (ja == "YES") {
      quitf(_wa, "Jury has a solution, but participant output NO");
    }
    // 双方都是 NO，本组数据通过
    return;
  }

  // Step 2. 参赛者输出 YES，检查构造方案
  // 读取 m 行边
  vector<vector<int>> adj(n + 1);
  for (int i = 0; i < m; ++i) {
    int u = ouf.readInt(1, n, "u");
    int v = ouf.readInt(1, n, "v");

    // 验证这条边是否在输入中存在
    int u_sorted = u, v_sorted = v;
    if (u_sorted > v_sorted) swap(u_sorted, v_sorted);

    auto it = input_edges.find({u_sorted, v_sorted});
    if (it == input_edges.end()) {
      quitf(_wa,
            "Output edge %d->%d is not present in input or used more times "
            "than provided",
            u, v);
    }
    input_edges.erase(it);

    // 构建有向图
    adj[u].push_back(v);
  }

  // 3. 预处理汇点（Sink Node）
  // 题目规则：如果没有出边，必须留在原地。相当于加一个自环。
  for (int i = 1; i <= n; ++i) {
    if (adj[i].empty()) {
      adj[i].push_back(i);
    }
  }

  // 4. 在乘积图上运行 BFS 验证是否会相遇
  // visited[u][v] 表示是否已访问和纱在 u，桔梗在 v 的状态
  vector<bitset<MAXN>> visited(n + 1);
  queue<pair<int, int>> q;

  visited[s][t] = true;
  q.emplace(s, t);

  while (!q.empty()) {
    auto [cur_u, cur_v] = q.front();
    q.pop();

    // 优化技巧：构建 v 的下一步可能位置的掩码
    bitset<MAXN> v_next_mask;
    for (int next_v : adj[cur_v]) {
      v_next_mask[next_v] = true;
    }

    // 遍历 u 的所有下一步
    for (int next_u : adj[cur_u]) {
      // 计算新的状态组合 (next_u, next_v) 中哪些还没被访问过
      // ~visited[next_u] 表示 next_u 这一行中未访问的列
      // 与 v_next_mask 进行与运算，直接得到需要加入队列的那些 next_v
      bitset<MAXN> new_states = v_next_mask & (~visited[next_u]);

      if (new_states.none()) continue;

      // 标记为已访问
      visited[next_u] |= new_states;

      // 遍历 bitset 中所有为 1 的位
      // _Find_first 和 _Find_next 是 GCC 内置扩展，但在 CP 环境（通常是
      // GCC）可用且高效 如果环境不支持，可用标准循环替代，但在 dense graph
      // 下会慢
      for (int next_v = new_states._Find_first(); next_v < MAXN;
           next_v = new_states._Find_next(next_v)) {
        if (next_u == next_v) {
          quitf(_wa, "Agents meet at intersection %d", next_u);
        }
        q.emplace(next_u, next_v);
      }
    }
  }

  // 如果遍历完都没有相遇
  if (ja == "NO") {
    quitf(_fail,
          "Participant found a valid solution but Jury said NO. Possible Jury "
          "Error.");
  }

  // 如果这里也没有返回，说明通过了这组数据的测试
}

int main(int argc, char* argv[]) {
  registerTestlibCmd(argc, argv);

  int t = inf.readInt();
  while (t--) {
    solve();
  }

  quitf(_ok, "All test cases passed");
}
