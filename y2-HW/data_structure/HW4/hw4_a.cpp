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
	void BFS(int v);
	void DFS(int v);
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
        for(int i=0 ; i<vertexs.size(); i++)
            adjacent_matrix.push_back(vector<int>(vertexs.size()));
    }
    if(adjacent_list.size()==0){
    	for(int i=0; i<vertexs.size(); i++)
    	    adjacent_list.push_back(list<vector<int>>());
    }
    adjacent_matrix[u][v]=weight;
    adjacent_list[u].push_back({v, weight});
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
void Graph::BFS(int v){
    if(v>=NumberVecterxs){ return; }
    bool *visited=new bool[NumberVecterxs];
    fill(visited, visited+NumberVecterxs, false);
    visited[v]=true;
    list<int> q;
    q.push_back(v);
    while(!q.empty()){
        v=q.front();
        cout<<v<<endl;
        q.pop_front();
        for(auto it: adjacent_list[v]){
            if(!visited[it[0]])
                q.push_back(it[0]);
                visited[it[0]]=true;
        }
    }
    delete[] visited;
}
void Graph::DFS(int v){
    if(v>=NumberVecterxs){ return; }
    bool *visited=new bool[NumberVecterxs];
    fill(visited, visited+NumberVecterxs, false);    
    list<int> q;
    q.push_back(v);
    while(!q.empty()){
        v=q.back();
        q.pop_back();        
        if(visited[v]) continue;
        cout<<v<<endl;
        visited[v]=true;        
        vector<int> neighbors;
        for(auto it: adjacent_list[v]){
            if(!visited[it[0]])
                neighbors.push_back(it[0]);
        }
        reverse(neighbors.begin(), neighbors.end());
        for(int neighbor: neighbors){
            q.push_back(neighbor);   
        }
    }
    delete[] visited;
}

int main(){
    int u, v;
    int weight;
    int N;
    int M;
    Graph g1;
    cin>>N>>M;
    for(int i=0; i<N; i++){
    	cin>>u;
    	g1.InsertVertex(u);
    }
    for(int j=0; j<M; j++){
    	cin>>u>>v;
    	g1.InsertEdge(u, v);
    }
    g1.PrintAdjacentMatrix();
    cout<<endl;
    g1.PrintAdjacentList();
    cout<<endl;    
    g1.BFS(0);
    cout<<endl;
    g1.DFS(0);
}