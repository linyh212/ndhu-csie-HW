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
	vector<tuple<int, int, int>> getEdge();
	vector<vector<int>> KruskalMST();
private:
	int NumberVecterxs;
	int NumberEdges;
	list<int> vertexs;
	vector<vector<int>> adjacent_matrix;
	vector<list<vector<int>>> adjacent_list;
	vector<int> parent, rank;
	int find(int x);
	void merge(int x, int y);
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
vector<tuple<int, int, int>> Graph::getEdge(){
    vector<tuple<int, int, int>> edges;
    for(int i=0; i<NumberVecterxs; i++){
        for(int j=i+1; j<NumberVecterxs; j++){
            if(adjacent_matrix[i][j]>0){
                edges.push_back({adjacent_matrix[i][j], i, j});
            }
        }
    }
    return edges;
}
int Graph::find(int x){ 
    return x==parent[x]? x:(parent[x]=find(parent[x])); 
}
void Graph::merge(int x, int y){
    int rootX=find(x);
    int rootY=find(y);
    if(rootX!=rootY){
        if(rank[rootX]>rank[rootY]) parent[rootY]=rootX;
        else if(rank[rootX]<rank[rootY]) parent[rootX] = rootY;
        else{
            parent[rootY]=rootX;
            rank[rootX]++;
        }
    }
}
vector<vector<int>> Graph::KruskalMST(){
    vector<tuple<int, int, int>> edges=getEdge();
    sort(edges.begin(), edges.end());
    parent.resize(NumberVecterxs);
    for(int i=0; i<NumberVecterxs; i++){
        parent[i]=i;
    }
    rank.resize(NumberVecterxs, 0);
    for(int i=0; i<NumberVecterxs; i++){ parent[i]=i; }
    vector<vector<int>> mst;
    for(int i=0; i<edges.size(); i++){
        int weight, u, v;
        tie(weight, u, v)=edges[i];
        if(find(u)!=find(v)){
            merge(u, v);
            mst.push_back({u, v});
        }
        if(mst.size()==NumberVecterxs-1){
            break;
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
    vector<vector<int>> mst=g2.KruskalMST();
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
