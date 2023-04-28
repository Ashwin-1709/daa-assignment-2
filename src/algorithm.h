/** \file */
#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <bits/stdc++.h>
#include "dbg.hh"
#include "typedefs.hh"

using namespace std;

const int64_t MAX_FLOW = INT64_MAX;
map<array<int , 2>, int>flow;

/// @brief Finds the maximum flow along the current path
/// @param source Source node
/// @param sink Destination node
/// @param residualCapacity Matrix storing the residual capacities between nodes
/// @param adj Graph represented by adjacency list
/// @return Returns a pair containing a vector of parents and the current maximum flow
auto find_simple_path (int64_t source, int64_t sink, vector<vector<int64_t>>&residualCapacity, vector<vector<int64_t>>&adj) -> pair<vector<int64_t>, int64_t>{
    int N = residualCapacity.size();
    vector<int64_t>parent(N + 1, -1);
    queue<array<int64_t, 2>>Q;

    parent[source] = source;
    Q.push({MAX_FLOW, source});
    // finds a simple path from sink with non zero flow
    while(!Q.empty()) {
        auto [cur_flow, cur_node] = Q.front();
        Q.pop();
        if(cur_node == sink)
            return {parent, cur_flow};
        for(auto &neighbours : adj[cur_node]) {
            // if residual capacity > 0 and not visited, visit it
            if(parent[neighbours] == -1 and residualCapacity[cur_node][neighbours] > 0) {
                parent[neighbours] = cur_node;
                // update the best flow
                int64_t best_flow = min(cur_flow, residualCapacity[cur_node][neighbours]);
                Q.push({best_flow, neighbours});
            }
        }
    }

    return {parent, 0LL};
};

/// @brief Updates the residual capacty matrix using best_flow
/// @param parent Vector of parent nodes
/// @param best_flow Maximum Flow along a path
/// @param s Source Node
/// @param t Destination Node
/// @param residualCapacity Matrix storing the residual capacities between nodes
/// @param set of forward edges in the graph
auto augment_path (vector<int64_t>parent, int64_t best_flow, int64_t s, int64_t t, vector<vector<int64_t>>&residualCapacity, set<array<int64_t, 2>>&forwardEdges) -> void {
    int cur_node = t;
    vector<int>path;
    // backtracking the path
    while(cur_node != s) {
        path.push_back(cur_node);
        int back_node = parent[cur_node];
        // if forward edge -> decrease the flow
        // if backward edge -> increase the flow
        if(forwardEdges.count({back_node, cur_node})) {
            flow[{back_node, cur_node}] -= best_flow;
        } else flow[{back_node, cur_node}] += best_flow;

        // update the residual capacities
        residualCapacity[back_node][cur_node] -= best_flow;
        residualCapacity[cur_node][back_node] += best_flow;
        cur_node = back_node;
    }
    reverse(begin(path), end(path));
};

/// @brief Finds the maximum flow in a flow network
/// @param s Source Node
/// @param t Destination Node
/// @param residualCapacity Matrix storing the residual capacities between nodes
/// @param adj Graph represented by adjacency list
/// @return Returns the maximum flow from source node to destination node
/// @param set of forward edges in the graph
auto findFlow(int64_t s, int64_t t, vector<vector<int64_t>>&residualCapacity, vector<vector<int64_t>>&adj, set<array<int64_t, 2>>&forwardEdges) -> int64_t {
    int64_t flow = 0;
    while(true) {
        auto [parent, best_flow] = find_simple_path(s, t, residualCapacity, adj);
        if(!best_flow)
            break;
        augment_path(parent, best_flow, s, t, residualCapacity, forwardEdges);
        flow += best_flow;
    }
    return flow;
};


auto minCut(int64_t s, int64_t t, vector<vector<int64_t>>&residualCapacity, vector<vector<int64_t>>&adj, set<array<int64_t, 2>>&forwardEdges) -> set<array<int64_t, 2>> {
    int N = (int)residualCapacity.size() + 1;
    // To find the mincut, we compute the reachability of every vertice from sink
    vector<int64_t> vis(N+1);
    set<array<int64_t, 2>> minCuts;
    auto findCut = [&](auto &&findCut, int64_t src) -> void{
        vis[src] = 1;
        for(auto &neighbours : adj[src]){
            if(residualCapacity[src][neighbours] > 0 && !vis[neighbours]){
                findCut(findCut, neighbours);
            }
        }
    };
    findCut(findCut, s);

    // Edge with a node reachable from sink and another not reachable -> s-t cut
    for(auto &[u , v] : forwardEdges) {
        if(vis[u] and !vis[v])
            minCuts.insert({u, v});
    }

    return minCuts;
};


#endif
