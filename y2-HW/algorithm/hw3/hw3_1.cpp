#include <bits/stdc++.h>
using namespace std;

struct Item{
    int weight, profit;
};
struct Node{
    int level, profit, weight;
    double bound;
};
double bound(int level, int weight, int profit, int W, const vector<Item> &items){
    int n = items.size();
    if (weight > W)
        return 0;
    double result = profit;
    int totalWeight = weight;
    for (int i = level; i < n; i++){
        if (totalWeight + items[i].weight <= W){
            totalWeight += items[i].weight;
            result += items[i].profit;
        }
        else{
            result += (W - totalWeight) * (double)items[i].profit / items[i].weight;
            break;
        }
    }
    return result;
}
struct CompareBound{
    bool operator()(const Node &a, const Node &b){
        return a.bound < b.bound;
    }
};

int dfsCount = 0, dfsBest = 0;
void KnapsackDFS(int level, int profit, int weight, int W, const vector<Item> &items){
    dfsCount++;
    int n = items.size();
    if (weight > W) return;
    if (level == n){
        if (profit > dfsBest) dfsBest = profit;
        return;
    }
    KnapsackDFS(level + 1, profit + items[level].profit, weight + items[level].weight, W, items);
    KnapsackDFS(level + 1, profit, weight, W, items);
}

int bfsCount = 0, bfsBest = 0;
void KnapsackBFS(int W, const vector<Item> &items){
    int n = items.size();
    queue<Node> q;
    q.push({0, 0, 0, 0});
    while (!q.empty()){
        Node node = q.front();
        q.pop();
        bfsCount++;
        if (node.level == n){
            if (node.weight <= W && node.profit > bfsBest) bfsBest = node.profit;
            continue;
        }
        if (node.weight + items[node.level].weight <= W)
            q.push({node.level + 1, node.profit + items[node.level].profit, node.weight + items[node.level].weight, 0});
        q.push({node.level + 1, node.profit, node.weight, 0});
    }
}

int bestCount = 0, bestBest = 0;
void KnapsackBestFS(int W, const vector<Item> &items){
    int n = items.size();
    priority_queue<Node, vector<Node>, CompareBound> pq;
    Node u, v;
    v.level = 0;
    v.profit = 0;
    v.weight = 0;
    v.bound = bound(0, 0, 0, W, items);
    pq.push(v);
    while (!pq.empty()){
        v = pq.top();
        pq.pop();
        bestCount++;
        if (v.level == n || v.bound <= bestBest) continue;

        u.level = v.level + 1;
        u.weight = v.weight + items[v.level].weight;
        u.profit = v.profit + items[v.level].profit;
        u.bound = bound(u.level, u.weight, u.profit, W, items);
        if (u.weight <= W && u.profit > bestBest) bestBest = u.profit;
        if (u.bound > bestBest) pq.push(u);

        u.weight = v.weight;
        u.profit = v.profit;
        u.bound = bound(u.level, u.weight, u.profit, W, items);
        if (u.bound > bestBest){
            u.level = v.level + 1;
            pq.push(u);
        }
    }
}

int main(){
    vector<int> itemCounts = {10, 15, 20, 25, 30};
    vector<int> weightRanges = {1, 10, 100, 1000, 10000};
    int seed = time(0);
    srand(seed);
    for (int n : itemCounts){
        for (int range : weightRanges){
            vector<Item> items(n);
            for (int i = 0; i < n; i++){
                items[i].weight = rand() % range + 1;
                items[i].profit = rand() % range + 1;
            }
            int W = range * n / 2;
            dfsCount = bfsCount = bestCount = 0;
            dfsBest = bfsBest = bestBest = 0;
            KnapsackDFS(0, 0, 0, W, items);
            KnapsackBFS(W, items);
            KnapsackBestFS(W, items);
            int totalNodes = 1 << n;
            cout << "Items: " << n << ", Weight range: [1-" << range << "], Capacity: " << W << endl;
            cout << "DFS: Best=" << dfsBest << ", Nodes visited=" << dfsCount << ", Total nodes=" << totalNodes << endl;
            cout << "BFS: Best=" << bfsBest << ", Nodes visited=" << bfsCount << ", Total nodes=" << totalNodes << endl;
            cout << "Best-First: Best=" << bestBest << ", Nodes visited=" << bestCount << ", Total nodes=" << totalNodes << endl;
            cout << "-----------------------------------" << endl;
        }
    }
    return 0;
}