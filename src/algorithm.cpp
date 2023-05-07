#include "algorithm.hpp"
#include "dbg.hpp"
#include "typedefs.hpp"
#include <algorithm>
#include <map>
#include <queue>

std::map<std::array<int, 2>, int> flow;

auto find_simple_path(i64 source, i64 sink,
                      std::vector<std::vector<i64>> &residualCapacity,
                      std::vector<std::vector<i64>> &adj)
    -> std::pair<std::vector<i64>, i64> {
    int N = residualCapacity.size();
    std::vector<i64> parent(N + 1, -1);
    std::queue<std::array<i64, 2>> Q;

    parent[source] = source;
    Q.push({MAX_FLOW, source});
    // finds a simple path from sink with non zero flow
    while (!Q.empty()) {
        auto [cur_flow, cur_node] = Q.front();
        Q.pop();
        if (cur_node == sink) {
            return {parent, cur_flow};
        }
        for (auto &neighbours : adj[cur_node]) {
            // if residual capacity > 0 and not visited, visit it
            if (parent[neighbours] == -1 and
                residualCapacity[cur_node][neighbours] > 0) {
                parent[neighbours] = cur_node;
                // update the best flow
                i64 best_flow =
                    std::min(cur_flow, residualCapacity[cur_node][neighbours]);
                Q.push({best_flow, neighbours});
            }
        }
    }

    return {parent, 0LL};
};

auto augment_path(std::vector<i64> parent, i64 best_flow, i64 s, i64 t,
                  std::vector<std::vector<i64>> &residualCapacity,
                  std::set<std::array<i64, 2>> &forwardEdges) -> void {
    int cur_node = t;
    std::vector<int> path;
    // backtracking the path
    while (cur_node != s) {
        path.push_back(cur_node);
        int back_node = parent[cur_node];
        // if forward edge -> decrease the flow
        // if backward edge -> increase the flow
        if (forwardEdges.count({back_node, cur_node})) {
            flow[{back_node, cur_node}] -= best_flow;
        } else
            flow[{back_node, cur_node}] += best_flow;

        // update the residual capacities
        residualCapacity[back_node][cur_node] -= best_flow;
        residualCapacity[cur_node][back_node] += best_flow;
        cur_node = back_node;
    }
    std::reverse(begin(path), end(path));
};

auto findFlow(i64 s, i64 t, std::vector<std::vector<i64>> &residualCapacity,
              std::vector<std::vector<i64>> &adj,
              std::set<std::array<i64, 2>> &forwardEdges) -> i64 {
    i64 flow = 0;
    while (true) {
        auto [parent, best_flow] =
            find_simple_path(s, t, residualCapacity, adj);
        if (!best_flow)
            break;
        augment_path(parent, best_flow, s, t, residualCapacity, forwardEdges);
        flow += best_flow;
    }
    return flow;
};

auto minCut(i64 s, i64 t, std::vector<std::vector<i64>> &residualCapacity,
            std::vector<std::vector<i64>> &adj,
            std::set<std::array<i64, 2>> &forwardEdges)
    -> std::set<std::array<i64, 2>> {
    int N = (int)residualCapacity.size() + 1;
    // To find the mincut, we compute the reachability of every vertice from
    // sink
    std::vector<i64> vis(N + 1);
    std::set<std::array<i64, 2>> minCuts;
    auto findCut = [&](auto &&findCut, i64 src) -> void {
        vis[src] = 1;
        for (auto &neighbours : adj[src]) {
            if (residualCapacity[src][neighbours] > 0 && !vis[neighbours]) {
                findCut(findCut, neighbours);
            }
        }
    };
    findCut(findCut, s);

    // Edge with a node reachable from sink and another not reachable -> s-t cut
    for (auto &[u, v] : forwardEdges) {
        if (vis[u] and !vis[v])
            minCuts.insert({u, v});
    }

    return minCuts;
};
