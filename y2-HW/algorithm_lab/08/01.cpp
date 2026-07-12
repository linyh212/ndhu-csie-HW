#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int N;
    cin >> N;
    // 建立 triangle，大小為 N x N
    vector<vector<int>> triangle(N);
    // 讀入三角形資料
    for (int i = 0; i < N; ++i) {
        triangle[i].resize(i + 1);
        for (int j = 0; j <= i; ++j) {
            cin >> triangle[i][j];
        }
    }
    // 從倒數第二層往上更新最大路徑和
    for (int i = N - 2; i >= 0; --i) {
        for (int j = 0; j <= i; ++j) {
            triangle[i][j] += max(triangle[i+1][j], triangle[i+1][j+1]);
        }
    }
    // triangle[0][0] 就是最大總和
    cout << triangle[0][0] << endl;
    return 0;
}
/*
input:
4
5
8 4
2 6 9
1 5 9 3

output:
28
*/