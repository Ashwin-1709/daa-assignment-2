/** \file */
#include "algorithm.h"
using namespace std;

/// @brief Outputs the maximum flow in a flow network
int32_t main() {
    int N , M;
    cin >> N >> M;
    vector<vector<int64_t>>adj(N + 1), residualCapacity(N + 1 , vector<int64_t>(N + 1));
    set<array<int64_t , 2>>forwardEdgeSet;
    for(int i = 0 ; i < M ; i++) {
        int64_t source, dest, capacity; 
        cin >> source >> dest >> capacity;
        forwardEdgeSet.insert({source, dest});
        adj[source].push_back(dest);
        adj[dest].push_back(source);
        residualCapacity[source][dest] += capacity;
    }

    int64_t flow = 0;
    int64_t s, t; cin >> s >> t; 
    // Ford Fulkerson Max flow
    cout << findFlow(s, t, residualCapacity, adj, forwardEdgeSet) << '\n';
}
