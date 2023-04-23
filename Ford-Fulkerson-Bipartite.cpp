#include "algorithm.h"

using namespace std;

int32_t main() {
    int N , M;
    cin >> N >> M;
    vector<vector<int64_t>>adj(N + 1), residualCapacity(N + 2 , vector<int64_t>(N + 2));

    for(int i = 0 ; i < M ; i++) {
        int64_t source, dest; 
        cin >> source >> dest;
        adj[source].push_back(dest);
        adj[dest].push_back(source);
    }

    vector<vector<int64_t>>bipAdj(N + 2), forwardBip(N + 2);
    int64_t source = 0, sink = N + 1;

    vector<int>col(N + 1, -1);
    auto dfs_col = [&](auto &&f , int cur, int c) ->void {
        col[cur] = c;
        for(auto &child : adj[cur]) {
            if(col[child] != -1)
                continue;
            f(f, child, c ^ 1);
        }
    };

    dfs_col(dfs_col, 1, 0);

    vector<int>red, blue;
    for(int node = 1; node <= N ; node++)
        col[node] ? red.push_back(node) : blue.push_back(node);
    

    for(auto &redNodes : red) {
        bipAdj[0].push_back(redNodes);
        bipAdj[redNodes].push_back(0);
        residualCapacity[0][redNodes]++;
        for(auto &blueNodes : adj[redNodes]) {
            if(col[blueNodes]) 
                continue;
            bipAdj[redNodes].push_back(blueNodes);
            bipAdj[blueNodes].push_back(redNodes);
            residualCapacity[redNodes][blueNodes]++;
            forwardBip[redNodes].push_back(blueNodes);
        }
    }

    for(auto &blueNodes : blue) {
        bipAdj[N + 1].push_back(blueNodes);
        bipAdj[blueNodes].push_back(N + 1);
        residualCapacity[blueNodes][N + 1]++;
    }

    auto flow = findFlow(0, N + 1, residualCapacity, bipAdj);
    cout << flow << '\n';
    
    set<array<int64_t , 2>>edges;
    for(auto &blueNodes : blue) {
        for(auto &redNodes : adj[blueNodes]) 
            if(residualCapacity[blueNodes][redNodes]) edges.insert({redNodes, blueNodes});
    }

    for(auto &[redNodes, blueNodes] : edges)
        cout << redNodes << ' ' << blueNodes << '\n';
}