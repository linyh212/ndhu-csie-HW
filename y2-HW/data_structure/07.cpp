#include<iostream>
#include<cstdlib>
#include<ctime>

#define SIZE 10

using namespace std;

class Node{
public:
	Node(){
		Node(0, 0);
	}
	Node(int r, int c){
		row=r;
		col=c;
	}
	int getRow(){ return row; }
	int getCol(){ return col; }
	void setRow(int r){
		if(r>=0&&r<SIZE)
			row=r;
	}
	void setCol(int c){
		if(c>=0&&c<SIZE)
			col=c;
	}
private:
	int col, row;
};

class List{
public:
	List(){
		top=0;
	}
	/*
	function addElement
	Insert an element from list
	*/
	void addElement(int r, int c){
		if(top<SIZE*SIZE){
			data[top].setRow(r);
			data[top].setCol(c);
			top++;
		}
	}
	/*
	function removeElement
	remove an element from list and return a pointer point to the element.
	If list is empty, return NULL.
	*/
	Node *removeElement(){
		if(top>0){
			return &data[--top];
		}
		return NULL;
	}
	void printList(){
		int j;
		for(j=0; j<top; j++){
			cout<<"("<<data[j].getRow()<<", "<<data[j].getCol()<<")"<<endl;
		}
	}
private:
	Node data[SIZE*SIZE];
	int top;
};

class Maze{
public:
	Maze(){
		initMaze(SIZE);
	}
	/*
	function initMaze
	Alocate a 2-D array with s * s sizes as the map of maze.
	Inside the maze where 0 represent empty space and 1 represent wall.
	[0][0] is start point and [s - 1][s - 1] is finish point.
	Randomly generate 20% wall in the maze.
	Make sure [0][0] and [s - 1][s - 1] are 0

	動態配置一個二維的陣列，大小是 s * s ，用這個陣列表示迷宮的地圖
	陣列中 0 的值表示可以移動的空間， 1 表示牆壁 
	[0][0] 的位置表示起點 [s - 1][s - 1] 的位置表示終點
	請在迷宮中加入 20% 的牆壁 
	然後確定起點跟終點都是可以移動的位置 
	*/
	void initMaze(int s){
		maze=new int*[s];
		for(int i=0; i<s; i++){
			maze[i]=new int[s];
		}
		int wallCount=(s*s)*0.2;
		for(int i=0; i<s; i++){
            for(int j=0; j<s; j++){
                maze[i][j]=0;
            }
        }
		srand(time(0));
		while(wallCount>0){
	        int r=rand()%s;
	        int c=rand()%s;
	        if((r!=0||c!=0)&&(r!=s-1||c!=s-1)&&maze[r][c]==0){
	            maze[r][c]=1;
	            wallCount--;
	        }
    	}
		maze[0][0]=0;
		maze[s-1][s-1]=0;
	}
	bool findPath(int **maze, int x, int y, int s, List &path){
		if(x<0||y<0||x>s-1||y>s-1||maze[x][y]!=0) return false;
		if(x==s-1&&y==s-1){
			path.addElement(x, y);
			return true;
		}
		maze[x][y]=2;
		if(findPath(maze, x+1, y, s, path)||findPath(maze, x, y+1, s, path)||findPath(maze, x-1, y, s, path)||findPath(maze, x, y-1, s, path)){
			path.addElement(x, y);
			return true;
		}
		return false;
	}
	/*
	function getPath
	This function will find a path between start point and finish point.
	Return a list content the path information inside.
	If there is no path between two point then the list will be empty.

	這個函數會找到起點到終點間的一條路徑
	回傳一個 list 包含著路徑的資訊
	如果找不到路徑的話 list 就會是空的 
	*/
	List *getPath(){
		List *path=new List();
		List reversePath;
		if(findPath(maze, 0, 0, SIZE, reversePath)){
			Node *node;
			while((node=reversePath.removeElement())!=nullptr){
				path->addElement(node->getRow(), node->getCol());
			} 
		}
		return path;
	}
	void printMaze(){
		int j, k;
		for(j=0; j<SIZE; j++){
			for(k=0; k<SIZE; k++){
				if(maze[j][k]==0) cout<<" ";
				else if(maze[j][k]==1) cout<<"*";
			}
			cout<<"\n";
		}
	}
private:
	int **maze;
};

int main(){
	Maze *maze=new Maze();
	maze->printMaze();
	maze->getPath()->printList();
}
