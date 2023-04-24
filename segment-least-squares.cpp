#include <bits/stdc++.h>
#include "dbg.hh"
using namespace std;

const double INF = 1e15;

double dist(double x, double y, array<double, 3>line) {
    if(line[1] == 0) 
        return (x + line[2]) * (x + line[2]);
    double y_pred = line[0] * x + line[2];
    return (y_pred - y) * (y_pred - y);
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
    if(N * sum_x2 - sum_x * sum_x == 0)
        return {1 , 0 , -sum_x / N};
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
    vector<int>optimal(N);

    auto f = [&](auto &&opt, int j) -> double {
        if(j < 0) return 0;
        if(vis[j])
            return dp[j];
        vis[j] = true;
        double cur_err = INF;
        for(int i = 0; i <= j ; i++) {
            double val = opt(opt, i - 1);
            if(val + C + e[i][j] < cur_err) {
                cur_err = val + C + e[i][j];
                optimal[j] = i;
            }
        }
        return dp[j] = cur_err;
    };

    cout << f(f , N - 1) << '\n';
    int cur_seg = N - 1;
    vector<vector<array<double , 2>>>segments;
    while(cur_seg > 0) {
        int from = optimal[cur_seg];
        vector<array<double , 2>>cur_segment;
        for(int j = from; j <= cur_seg ; j++)
            cur_segment.push_back(points[j]);
        segments.push_back(cur_segment);
        cur_seg = from - 1;
    }

    reverse(begin(segments), end(segments));
    cout << segments.size() << '\n';
    for(auto &p : segments) {
        cout << p.size() << '\n';
        for(auto &[x, y] : p)
            cout << x << ' ' << y << '\n';
        auto cur_line = get_line(p);
        cout << cur_line[0] << ' ' << cur_line[1] << ' ' << cur_line[2] << '\n';
    }
}