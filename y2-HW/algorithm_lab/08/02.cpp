#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int N, fee;
    cin >> N >> fee;    
    vector<int> prices(N);
    for (int i = 0; i < N; ++i) {
        cin >> prices[i];
    }
    if (N == 0) {
        cout << 0 << endl;
        return 0;
    }
    int cash = 0;              // 不持有股票的最大利潤
    int hold = -prices[0];     // 持有股票的最大利潤（初始為買第一天的股票）
    for (int i = 1; i < N; ++i) {
        int new_cash = max(cash, hold + prices[i] - fee);
        int new_hold = max(hold, cash - prices[i]);
        cash = new_cash;
        hold = new_hold;
    }
    cout << cash << endl;
    return 0;
}
/*
intput:
6 2
1 3 2 8 4 9

output:
8
*/