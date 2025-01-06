#include<bits/stdc++.h>
using namespace std;

class Graph{
public:
	Graph();
	void InsertVertex (int v);
	void InsertEdge(int u, int v, int weight);
	bool IsEmpty();
	list<vector<int>> Adjacent_List(int u);
	void PrintAdjacentMatrix();
	void PrintAdjacentList();
	vector<vector<int>> PrimMST(int vertex);
private:
	int NumberVecterxs;
	int NumberEdges;
	list<int> vertexs;
	vector<vector<int>> adjacent_matrix;
	vector<list<vector<int>>> adjacent_list;
};
Graph::Graph(){
    NumberVecterxs=NumberEdges=0;
}
bool Graph::IsEmpty(){
    return (vertexs.size()==0);
}
list<vector<int>> Graph::Adjacent_List(int i){
    return adjacent_list[i];
}
void Graph::InsertVertex(int v){
    vertexs.push_back(v);
    NumberVecterxs++;
}
void Graph::InsertEdge(int u, int v, int weight=1){
    if(adjacent_matrix.size()==0){
    	for(int i=0; i<vertexs.size(); i++)
    	    adjacent_matrix.push_back(vector<int>(vertexs.size()));
    }
    if(adjacent_list.size()==0){
    	for(int i=0; i<vertexs.size(); i++)
    	    adjacent_list.push_back(list<vector<int>>());
    }
    adjacent_matrix[u][v]=adjacent_matrix[v][u]=weight;
    adjacent_list[u].push_back({v, weight});
    adjacent_list[v].push_back({u, weight});
    NumberEdges++;
}
void Graph::PrintAdjacentMatrix(){
    for(int i=0; i<vertexs.size(); i++){
    	for(int j=0; j<vertexs.size(); j++){
    	    if(adjacent_matrix[i][j])
    		    cout<<"("<<i<<", "<<j<<", "<<adjacent_matrix[i][j]<<")"<<endl;
    	}
    }
}
void Graph::PrintAdjacentList(){
    for(int i=0; i<vertexs.size(); i++){
    	for(auto it: adjacent_list[i]){
    	    cout<<"("<<i<<", "<<it[0]<<", "<<it[1]<<")"<<endl;
    	}
    }
}
vector<vector<int>> Graph::PrimMST(int vertex){
    vector<bool> visited(NumberVecterxs, false);
    vector<int> minWeight(NumberVecterxs, 1e9);
    vector<int> parent(NumberVecterxs, -1);
    vector<vector<int>> mst;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, vertex});
    while(!pq.empty()){
        int u=pq.top().second;
        pq.pop();        
        if(visited[u]){ continue; }        
        visited[u]=true;        
        if(parent[u]!=-1){
            mst.push_back({parent[u], u, minWeight[u]});
        }        
        for(auto &neighbor : adjacent_list[u]){
            int v=neighbor[0];
            int weight=neighbor[1];
            if(!visited[v]&&weight<minWeight[v]){
                minWeight[v]=weight;
                parent[v]=u;
                pq.push({weight, v});
            }
        }
    }
    return mst;
}

int main(){
    int u, v;
    int weight;
    int N;
    int M;
    Graph g2;
    cin>>N>>M;
    for(int i=0; i<N; i++){
    	cin>>u;
    	g2.InsertVertex(u);
    }
    for(int j=0; j<M; j++){
    	cin>>u>>v>>weight;
    	g2.InsertEdge(u, v, weight);
    }
    g2.PrintAdjacentMatrix();
    g2.PrintAdjacentList();
    cout<<endl;    
    vector<vector<int>> mst=g2.PrimMST(0);
    for(auto &edge: mst){
        if(edge[0]>edge[1]) swap(edge[0], edge[1]);
    }
    sort(mst.begin(), mst.end(), [](const vector<int> &a, const vector<int> &b){
        return a[0]<b[0]||(a[0]==b[0]&&a[1]<b[1]);
    });
    for(const auto &edge: mst){
        cout<<edge[0]<<" "<<edge[1]<<endl;
    }
    return 0;
}