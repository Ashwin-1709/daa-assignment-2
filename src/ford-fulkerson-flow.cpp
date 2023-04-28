/** \file */
#include "algorithm.hpp"
#include "typedefs.hpp"

/// @brief Outputs the maximum flow in a flow network
int32_t main() {
    int N, M;
    std::cin >> N >> M;
    std::vector<std::vector<i64>> adj(N + 1),
        residualCapacity(N + 1, std::vector<i64>(N + 1));
    std::set<std::array<i64, 2>> forwardEdgeSet;
    for (int i = 0; i < M; i++) {
        i64 source, dest, capacity;
        std::cin >> source >> dest >> capacity;
        forwardEdgeSet.insert({source, dest});
        adj[source].push_back(dest);
        adj[dest].push_back(source);
        residualCapacity[source][dest] += capacity;
    }

    i64 flow = 0;
    i64 s, t;
    std::cin >> s >> t;
    // Ford Fulkerson Max flow
    std::cout << "Flow : " << '\n';
    std::cout << findFlow(s, t, residualCapacity, adj, forwardEdgeSet) << '\n';

    // min cut
    auto minCuts = minCut(s, t, residualCapacity, adj, forwardEdgeSet);
    std::cout << "Min cuts : ";
    std::cout << minCuts.size() << '\n';

    for (auto &[u, v] : minCuts) {
        std::cout << u << ' ' << v << '\n';
    }
}
