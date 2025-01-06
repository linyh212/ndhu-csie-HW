#include<bits/stdc++.h>
using namespace std;

class Graph{
public:
    Graph();
	void InsertVertex(int v);
	void InsertEdge(int u, int v, int weight);
	bool IsEmpty();
	list<vector<int>> Adjacent_List(int u);
	void PrintAdjacentMatrix();
	void PrintAdjacentList();
	vector<vector<int>> ShortestPaths(int src);
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
    		    cout<<"("<<i<<","<<j<<", "<<adjacent_matrix[i][j]<<")"<<endl;
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
vector<vector<int>> Graph::ShortestPaths(int src){
    vector<int> dist(NumberVecterxs, 1e8);
    vector<int> parent(NumberVecterxs, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[src]=0;
    pq.push({0, src});
    while(!pq.empty()){
        int u=pq.top().second;
        pq.pop();
        for(auto &neighbor: adjacent_list[u]){
            int v=neighbor[0];
            int weight=neighbor[1];
            if(dist[u]+weight<dist[v]){
                dist[v]=dist[u]+weight;
                parent[v]=u;
                pq.push({dist[v], v});
            }
        }
    }
    vector<vector<int>> paths(NumberVecterxs);
    for(int i=0;i<NumberVecterxs;i++){
        if(i==src){
            paths[i].push_back(src);
        }
        else if(parent[i]!=-1){
            int curr=i;
            while(curr!=-1){
                paths[i].push_back(curr);
                curr=parent[curr];
            }
            reverse(paths[i].begin(), paths[i].end());
        }
    }
    return paths;
}

int main(){
    int u,v;
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
    	g2.InsertEdge(u,v,weight);
    }
    g2.PrintAdjacentMatrix();
    g2.PrintAdjacentList();
    cout<<endl;
    int start;
    cout<<"enter the starting point: ";
    cin>>start;
    vector<vector<int>> path=g2.ShortestPaths(start);
    for(int i=0;i<N;i++){
        if(i==start){ continue; }
        if(!path[i].empty()){
            for(int v:path[i]){
                cout<<v<<" ";
            }
        }
        else{
            cout<<"no path";
        }
        cout<<endl;
    }
    return 0;
}
