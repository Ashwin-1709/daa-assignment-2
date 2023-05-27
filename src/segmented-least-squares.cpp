/** \file */
#include "dbg.hpp"
#include <bits/stdc++.h>

const double INF = 1e15;

/// @brief Calculates the squared error value for a point
/// @param x x-coordinate of a point
/// @param y y-coordinate of a point
/// @param line Possible best fit line
/// @return Returns the squared error
auto dist(double x, double y, std::array<double, 3> line) -> double {
    if (line[1] == 0)
        return (x + line[2]) * (x + line[2]);
    double const y_pred = line[0] * x + line[2];
    return (y_pred - y) * (y_pred - y);
}

/// @brief Calculates the slope and intercept of possible best fit line
/// @param points vector of points
/// @return Returns the possible best fit line
auto get_line(std::vector<std::array<double, 2>> &points)
    -> std::array<double, 3> {
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
    for (auto &[x, y] : points) {
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_x2 += (x * x);
    }
    auto const N = (double)points.size();
    if (N * sum_x2 - sum_x * sum_x == 0)
        return {1, 0, -sum_x / N};
    double slope = (N * sum_xy - sum_x * sum_y);
    slope = slope / (N * sum_x2 - sum_x * sum_x);

    double const intercept = (sum_y - slope * sum_x) / N;
    return {slope, -1, intercept};
}

/// @brief Calculates the total squared error over a set of points
/// @param points std::vector or points
/// @param line Possible best fit line
/// @return Returns the error value
auto error(std::vector<std::array<double, 2>> &points,
           std::array<double, 3> &line) -> double {
    double err = 0;
    for (auto &[x, y] : points)
        err += dist(x, y, line);
    return err;
}

/// @brief Outputs the best fit line segments found using
/// segmented least squares algorithm
auto main() -> int {
    usize N;
    std::cin >> N;
    double C;
    std::cin >> C;
    std::vector<std::array<double, 2>> points;
    for (usize i = 0; i < N; i++) {
        double x, y;
        std::cin >> x >> y;
        points.push_back({x, y});
    }

    sort(begin(points), end(points));

    std::vector<std::vector<double>> e(N, std::vector<double>(N));
    for (usize i = 0; i < N; i++) {
        std::vector<std::array<double, 2>> p = {points[i]};
        e[i][i] = 0;
        for (usize j = i + 1; j < N; j++) {
            p.push_back(points[j]);
            auto new_l = get_line(p);
            e[i][j] = error(p, new_l);
        }
    }

    std::vector<double> dp(N);
    std::vector<bool> vis(N);
    std::vector<u64> optimal(N);

    auto f = [&](auto &&opt, usize j) -> double {
        if (j < 0) {
            return 0;
        }
        if (vis[j]) {
            return dp[j];
        }
        vis[j] = true;
        double cur_err = INF;
        for (usize i = 0; i <= j; i++) {
            double const val = opt(opt, i - 1);
            if (val + C + e[i][j] < cur_err) {
                cur_err = val + C + e[i][j];
                optimal[j] = i;
            }
        }
        return dp[j] = cur_err;
    };

    std::cout << f(f, N - 1) << '\n';
    u64 cur_seg = N - 1;
    std::vector<std::vector<std::array<double, 2>>> segments;
    while (cur_seg > 0) {
        u64 const from = optimal[cur_seg];
        std::vector<std::array<double, 2>> cur_segment;
        for (u64 j = from; j <= cur_seg; j++) {
            cur_segment.push_back(points[j]);
        }
        segments.push_back(cur_segment);
        cur_seg = from - 1;
    }

    reverse(begin(segments), end(segments));
    std::cout << segments.size() << '\n';
    for (auto &p : segments) {
        std::cout << p.size() << '\n';
        for (auto &[x, y] : p) {
            std::cout << x << ' ' << y << '\n';
        }
        auto cur_line = get_line(p);
        std::cout << cur_line[0] << ' ' << cur_line[1] << ' ' << cur_line[2]
                  << '\n';
    }
}
