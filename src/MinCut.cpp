#include "algorithm.h"
using namespace std;
 
int32_t main() {
    int N , M;
    cin >> N >> M;
    vector<vector<int64_t>>adj(N + 1), residualCapacity(N + 1 , vector<int64_t>(N + 1));
    set<array<int64_t , 2>>forwardEdgeSet, minCut;

    for(int i = 0 ; i < M ; i++) {
        int64_t source, dest, capacity;
        cin >> source >> dest >> capacity;
        adj[source].push_back(dest);
        adj[dest].push_back(source);

        forwardEdgeSet.insert({source, dest});

        residualCapacity[source][dest] += capacity;
    }
    
    int64_t s = 1, t = N; cin >> s >> t;
    // Max flow on the graph
    auto flow = findFlow(s, t, residualCapacity, adj, forwardEdgeSet);  
    cout << "Flow : " << flow << '\n';
    
    vector<int64_t> vis(N+1);

    // To find the mincut, we compute the reachability of every vertice from sink
    auto findCut = [&](auto &&findCut, int64_t src) -> void{
        vis[src] = 1;
        for(auto &neighbours:adj[src]){
            if(residualCapacity[src][neighbours] > 0 && !vis[neighbours]){
                findCut(findCut, neighbours);
            }
        }
    };
    findCut(findCut, s);

    // Edge with a node reachable from sink and another not reachable -> s-t cut
    for(int i = 1 ; i <= N ; i++) {
        for(int j = 1 ; j <= N ; j++) {
            if(i == j) continue;
            if(vis[i] and !vis[j] and forwardEdgeSet.count({i, j}))
                minCut.insert({i , j});
        }
    }

    cout << minCut.size() << '\n';

    for(auto &[u , v] : minCut)
        cout << u << ' ' << v << '\n';
}