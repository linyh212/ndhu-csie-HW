#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Bottom-up DP method
int knapsackBottomUp(int W, const vector<int> &weights, const vector<int> &profits) {
    int n = weights.size();
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (weights[i - 1] <= w) dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - weights[i - 1]] + profits[i - 1]);
            else dp[i][w] = dp[i - 1][w];
        }
    }
    return dp[n][W];
}
// Top-down DP method with memoization
int knapsackTopDownHelper(int i, int W, const vector<int> &weights, const vector<int> &profits, vector<vector<int>> &memo) {
    if (i < 0 || W <= 0) return 0;
    if (memo[i][W] != -1) return memo[i][W];
    if (weights[i] > W) memo[i][W] = knapsackTopDownHelper(i - 1, W, weights, profits, memo);
    else {
        memo[i][W] = max(knapsackTopDownHelper(i - 1, W, weights, profits, memo),
        profits[i] + knapsackTopDownHelper(i - 1, W - weights[i], weights, profits, memo));
    }
    return memo[i][W];
}
int knapsackTopDown(int W, const vector<int> &weights, const vector<int> &profits) {
    int n = weights.size();
    vector<vector<int>> memo(n, vector<int>(W + 1, -1));
    return knapsackTopDownHelper(n - 1, W, weights, profits, memo);
}
// Greedy method
int knapsackGreedy(int W, vector<int> weights, vector<int> profits) {
    int n = weights.size();
    vector<pair<double, int>> ratio(n);
    for (int i = 0; i < n; i++) {
        ratio[i] = {static_cast<double>(profits[i]) / weights[i], i};
    }
    sort(ratio.rbegin(), ratio.rend());
    int totalProfit = 0;
    for (size_t i = 0; i < ratio.size(); i++) {
        double r = ratio[i].first;
        int idx = ratio[i].second;
        if (weights[idx] <= W) {
            W -= weights[idx];
            totalProfit += profits[idx];
        }
        else {
            totalProfit += profits[idx] * W / weights[idx];
            break;
        }
    }
    return totalProfit;
}
// Test and compare methods
void testKnapsackMethods() {
    vector<int> itemCounts = {10, 20, 30};
    vector<int> weightRanges = {1, 10, 100, 1000};
    for (int n : itemCounts) {
        for (int range : weightRanges) {
            vector<int> weights(n), profits(n);
            for (int i = 0; i < n; i++) {
                weights[i] = rand() % range + 1;
                profits[i] = rand() % range + 1;
            }
            int W = range * n / 2;
            cout << "Items: " << n << ", Weight range: [1-" << range << "], Capacity: " << W << endl;
            auto start = high_resolution_clock::now();
            int bottomUpResult = knapsackBottomUp(W, weights, profits);
            auto end = high_resolution_clock::now();
            cout << "Bottom-up DP result: " << bottomUpResult << ", Time: " << duration_cast<nanoseconds>(end - start).count() << " ms" << endl;
            start = high_resolution_clock::now();
            int topDownResult = knapsackTopDown(W, weights, profits);
            end = high_resolution_clock::now();
            cout << "Top-down DP result: " << topDownResult << ", Time: " << duration_cast<nanoseconds>(end - start).count() << " ms" << endl;
            start = high_resolution_clock::now();
            int greedyResult = knapsackGreedy(W, weights, profits);
            end = high_resolution_clock::now();
            cout << "Greedy result: " << greedyResult << ", Time: " << duration_cast<nanoseconds>(end - start).count() << " ms" << endl;
            cout << "-----------------------------------" << endl;
        }
    }
}

int main() {
    testKnapsackMethods();
    return 0;
}