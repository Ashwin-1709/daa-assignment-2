/** \file */
#include "algorithm.hpp"
#include "typedefs.hpp"

/// @brief Outputs the Bipartite Matching for a graph using Ford Fulkerson
/// Algorithm
auto main() -> int {
    usize N, M;
    std::cin >> N >> M;
    std::vector<std::vector<u64>> adj(N + 1),
        residualCapacity(N + 2, std::vector<u64>(N + 2));
    std::set<std::array<u64, 2>> forwardEdgeSet, matchingEdges;
    for (usize i = 0; i < M; i++) {
        u64 source, dest;
        std::cin >> source >> dest;
        forwardEdgeSet.insert({source, dest});
        adj[source].push_back(dest);
        adj[dest].push_back(source);
    }

    std::vector<std::vector<u64>> bipAdj(N + 2), forwardBip(N + 2);

    // Coloring the bipartite graph using a depth first search
    std::vector<std::optional<u64>> col(N + 1, std::nullopt);
    auto dfs_col = [&](auto &&f, u64 cur, u64 c) -> void {
        col[cur] = c;
        for (auto &child : adj[cur]) {
            if (col[child].has_value()) {
                continue;
            }
            f(f, child, c ^ 1);
        }
    };

    for (usize i = 1; i <= N; i++) {
        if (col[i] == -1) {
            dfs_col(dfs_col, i, 0);
        }
    }

    // After coloring vertices are split in two sets, blue and red
    std::vector<u64> red, blue;
    for (usize node = 1; node <= N; node++) {
        col[node] ? red.push_back(node) : blue.push_back(node);
    }

    // We create a dummy souce and add edge from soucr to red nodes
    for (auto &redNodes : red) {
        bipAdj[0].push_back(redNodes);
        bipAdj[redNodes].push_back(0);
        residualCapacity[0][redNodes]++;
        for (auto &blueNodes : adj[redNodes]) {
            if (col[blueNodes]) {
                continue;
            }
            bipAdj[redNodes].push_back(blueNodes);
            bipAdj[blueNodes].push_back(redNodes);
            residualCapacity[redNodes][blueNodes]++;
            forwardBip[redNodes].push_back(blueNodes);
        }
    }

    // similarly dummy sink, add edges from blue nodes to the sink
    // edge capacity -> 1
    for (auto &blueNodes : blue) {
        bipAdj[N + 1].push_back(blueNodes);
        bipAdj[blueNodes].push_back(N + 1);
        residualCapacity[blueNodes][N + 1]++;
    }

    // Find th
    auto flow = findFlow(0, N + 1, residualCapacity, bipAdj, forwardEdgeSet);
    std::cout << flow << '\n';

    for (usize i = 1; i <= N; i++) {
        for (auto &forwardEdge : forwardBip[i]) {
            if (residualCapacity[i][forwardEdge] == 0) {
                matchingEdges.insert({i, forwardEdge});
            }
        }
    }

    for (auto &[redNodes, blueNodes] : matchingEdges) {
        std::cout << redNodes << ' ' << blueNodes << '\n';
    }
}
