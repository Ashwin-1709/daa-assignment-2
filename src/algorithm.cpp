#include "algorithm.hpp"
#include "dbg.hpp"
#include "typedefs.hpp"
#include <algorithm>
#include <map>
#include <optional>
#include <queue>

std::map<std::array<u64, 2>, u64> flow;

auto find_simple_path(u64 source, u64 sink,
                      std::vector<std::vector<u64>> &residualCapacity,
                      std::vector<std::vector<u64>> &adj)
    -> std::pair<std::vector<std::optional<u64>>, u64> {
    usize const N = residualCapacity.size();
    std::vector<std::optional<u64>> parent(N + 1, std::nullopt);
    std::queue<std::pair<u64, u64>> Q;

    parent[source] = std::make_optional(source);
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
            if (!parent[neighbours].has_value() and
                residualCapacity[cur_node][neighbours] > 0) {
                parent[neighbours] = std::make_optional(cur_node);
                // update the best flow
                u64 const best_flow =
                    std::min(cur_flow, residualCapacity[cur_node][neighbours]);
                Q.push({best_flow, neighbours});
            }
        }
    }

    return {parent, 0LL};
};

auto augment_path(std::vector<std::optional<u64>> parent, u64 best_flow, u64 s,
                  u64 t, std::vector<std::vector<u64>> &residualCapacity,
                  std::set<std::array<u64, 2>> &forwardEdges) -> void {
    u64 cur_node = t;
    std::vector<u64> path;
    // backtracking the path
    while (cur_node != s) {
        path.push_back(cur_node);
        u64 const back_node = parent[cur_node].value_or(0);
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

auto findFlow(u64 s, u64 t, std::vector<std::vector<u64>> &residualCapacity,
              std::vector<std::vector<u64>> &adj,
              std::set<std::array<u64, 2>> &forwardEdges) -> u64 {
    u64 flow_value = 0;
    while (true) {
        auto [parent, best_flow] =
            find_simple_path(s, t, residualCapacity, adj);
        if (!best_flow)
            break;
        augment_path(parent, best_flow, s, t, residualCapacity, forwardEdges);
        flow_value += best_flow;
    }
    return flow_value;
};

auto minCut(u64 s, std::vector<std::vector<u64>> &residualCapacity,
            std::vector<std::vector<u64>> &adj,
            std::set<std::array<u64, 2>> &forwardEdges)
    -> std::set<std::array<u64, 2>> {
    u64 const N = residualCapacity.size() + 1;
    // To find the mincut, we compute the reachability of every vertice from
    // sink
    std::vector<u64> vis(N + 1);
    std::set<std::array<u64, 2>> minCuts;
#pragma GCC diagnostic ignored "-Wshadow"
    auto findCut = [&](auto &&findCut, u64 src) -> void {
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
