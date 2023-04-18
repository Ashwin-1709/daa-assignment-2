#include <bits/stdc++.h>
#include "dbg.hh"

using namespace std;

const int64_t MAX_FLOW = INT64_MAX;

int32_t main() {
    int N , M;
    cin >> N >> M;
    vector<vector<int64_t>>adj(N + 1), residualCapacity(N + 1 , vector<int64_t>(N + 1));

    for(int i = 0 ; i < M ; i++) {
        int64_t source, dest, capacity; 
        cin >> source >> dest >> capacity;
        adj[source].push_back(dest);
        adj[dest].push_back(source);
        residualCapacity[source][dest] = capacity;
    }

    int64_t flow = 0;
    int64_t s, t; cin >> s >> t; 

    auto find_simple_path = [&](int64_t source, int64_t sink) -> pair<vector<int>, int64_t>{
        vector<int>parent(N + 1, -1);
        queue<array<int64_t, 2>>Q;

        parent[source] = source;
        Q.push({MAX_FLOW, source});

        while(!Q.empty()) {
            auto [cur_flow, cur_node] = Q.front();
            Q.pop();
            if(cur_node == sink)
                return {parent, cur_flow};
            for(auto &neighbours : adj[cur_node]) {
                if(parent[neighbours] == -1 and residualCapacity[cur_node][neighbours] > 0) {
                    parent[neighbours] = cur_node;
                    int64_t best_flow = min(cur_flow, residualCapacity[cur_node][neighbours]);
                    Q.push({best_flow, neighbours});
                }
            }
        }

        return {parent, 0LL};
    };

    
    auto augment_path = [&](vector<int>parent, int64_t best_flow) -> void {
        int cur_node = t;
        while(cur_node != s) {
            int back_node = parent[cur_node];
            residualCapacity[back_node][cur_node] -= best_flow;
            residualCapacity[cur_node][back_node] += best_flow;
            cur_node = back_node;
        }
    };

    while(true) {
        auto [parent, best_flow] = find_simple_path(s, t);
        if(!best_flow)
            break;
        augment_path(parent, best_flow);
        flow += best_flow;
    }

    cout << flow << '\n';
}