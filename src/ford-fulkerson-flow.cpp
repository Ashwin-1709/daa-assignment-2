/** \file */
#include "algorithm.hpp"
#include "typedefs.hpp"

/// @brief Outputs the maximum flow in a flow network
auto main() -> int {
    usize N, M;
    std::cin >> N >> M;
    std::vector<std::vector<u64>> adj(N + 1),
        residualCapacity(N + 1, std::vector<u64>(N + 1));
    std::set<std::array<u64, 2>> forwardEdgeSet;
    for (usize i = 0; i < M; i++) {
        u64 source, dest, capacity;
        std::cin >> source >> dest >> capacity;
        forwardEdgeSet.insert({source, dest});
        adj[source].push_back(dest);
        adj[dest].push_back(source);
        residualCapacity[source][dest] += capacity;
    }

    u64 s, t;
    std::cin >> s >> t;
    // Ford Fulkerson Max flow
    std::cout << "Flow : " << '\n';
    std::cout << findFlow(s, t, residualCapacity, adj, forwardEdgeSet) << '\n';

    // min cut
    auto minCuts = minCut(s, residualCapacity, adj, forwardEdgeSet);
    std::cout << "Min cuts : ";
    std::cout << minCuts.size() << '\n';

    for (auto &[u, v] : minCuts) {
        std::cout << u << ' ' << v << '\n';
    }
}
