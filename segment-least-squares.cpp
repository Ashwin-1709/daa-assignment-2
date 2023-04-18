#include <bits/stdc++.h>
#include "dbg.hh"
using namespace std;

const double INF = 1e15;

double dist(double x, double y, array<double, 3>line) {
    double d = (line[0] * x + line[1] * y + line[2]) / (sqrt(line[0] * line[0] + line[1] * line[1]));
    return d * d;
}

array<double, 3>get_line(vector<array<double, 2>>&points) {
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
    for(auto &[x, y] : points){
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_x2 += (x * x);
    }
    uint32_t N = points.size();
    double slope = (N * sum_xy - sum_x * sum_y);
    slope = slope / (N * sum_x2 - sum_x * sum_x);

    double intercept = (sum_y - slope * sum_x) / N;
    return {slope, -1, intercept};
}

double error(vector<array<double, 2>>&points, array<double, 3>&line) {
    double err = 0;
    for(auto &[x , y] : points)
        err += dist(x, y, line);
    return err;
}

int32_t main() {
    int N; cin >> N;
    double C; cin >> C;
    vector<array<double, 2>>points;
    for(int i = 0 ; i < N ; i++) {
        double x, y; cin >> x >> y;
        points.push_back({x, y});
    }

    sort(begin(points), end(points));
    
    vector<vector<double>>e(N, vector<double>(N));
    for(int i = 0 ; i < N ; i++) {
        vector<array<double, 2>>p = {points[i]};
        e[i][i] = 0;
        for(int j = i + 1 ; j < N ; j++) {
            p.push_back(points[j]);
            auto new_l = get_line(p);
            e[i][j] = error(p, new_l); 
        }
    }

    vector<double>dp(N);
    vector<bool>vis(N);

    auto f = [&](auto &&opt, int j) -> double {
        if(j < 0) return 0;
        if(vis[j])
            return dp[j];
        vis[j] = true;
        double cur_err = INF;
        int taken = -1;
        for(int i = 0; i <= j ; i++) {
            double val = opt(opt, i - 1);
            if(val + C + e[i][j] < cur_err) {
                cur_err = val + C + e[i][j];
                taken = i;
            }
        }
        return dp[j] = cur_err;
    };

    cout << f(f , N - 1) << '\n';
    dbg(dp);

}