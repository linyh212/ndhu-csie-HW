#include<iostream>
#include<cstdlib>
#include<ctime>
#include <vector>

using namespace std;

#define SIZE 10
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

class Grid{
public:
	Grid(){
		state=0;
        dir[UP]=nullptr;
        dir[DOWN]=nullptr;
        dir[LEFT]=nullptr;
        dir[RIGHT]=nullptr;
	}
	Grid(int s){
		state=s;
		dir[UP]=nullptr;
		dir[DOWN]=nullptr;
		dir[LEFT]=nullptr;
		dir[RIGHT]=nullptr;
	}
	Grid *getDir(int d){ return dir[d]; }
	int getState(){ return state; }
	void setDir(int d, Grid *g){ dir[d]=g; }
	void setState(int s){ state=s; }
private:
	Grid *dir[4];
	int state;
};

struct List{
public:
	List(){
		top=0;
	}
	/*
	Insert an element from top
	*/
	void addElement(Grid *g){
		if(top<SIZE*SIZE) data[top++]=g;
	}
	/*
	remove an element from top and return a pointer point to the element.
	If list is empty, return NULL.
	*/
	Grid *removeElement(){
		if(top>0) return data[--top];
		return NULL;
	}
	void printPath(){
		int j;
		for(j=1; j<top; j++){
			if(data[j]==data[j-1]->getDir(UP)){
				cout<<"UP\n";
			}
			else if(data[j]==data[j-1]->getDir(DOWN)){
				cout<<"DOWN\n";
			}
			else if(data[j]==data[j-1]->getDir(LEFT)){
				cout<<"LEFT\n";
			}
			else if(data[j]==data[j-1]->getDir(RIGHT)){
				cout<<"RIGHT\n";
			}
		}
	}
private:
	Grid *data[SIZE*SIZE];
	int top;
};

class Maze{
public:
	Maze(){
		initMaze(SIZE);
	}
	/*
	function initMaze
	Alocate a 2-D link list with s * s sizes as the map of maze.
	Inside the maze enery gird with state 0 represent empty space and 1 represent wall.
	The gird in left top is start point and right bottom is finish point.
	Randomly generate 20% wall in the maze.
	Make sure start point and finish point are 0

	動態配置一個二維的鏈結串列，大小是 s * s，用這個鏈結串列表示迷宮的地圖
	節點中 0 的值表示可以移動的空間， 1 表示牆壁 
	左上角的位置表示起點，右下角的位置表示終點
	請在迷宮中加入 20% 的牆壁 
	然後確定起點跟終點都是可以移動的位置 
	*/
	void initMaze(int s){
		maze=new Grid*[s];
		for(int i=0; i<s; i++){
			maze[i]=new Grid[s];
		}
		int wallCount=(s*s)*0.2;
		for(int i=0; i<s; i++){
            for(int j=0; j<s; j++){
                maze[i][j].setState(0);
                if(i>0) maze[i][j].setDir(UP, &maze[i-1][j]);
                if(i<s-1) maze[i][j].setDir(DOWN, &maze[i+1][j]);
                if(j>0) maze[i][j].setDir(LEFT, &maze[i][j-1]);
                if(j<s-1) maze[i][j].setDir(RIGHT, &maze[i][j+1]);
            }
        }		
		srand(time(0));
		while(wallCount>0){
	        int x=rand()%s;
	        int y=rand()%s;
	        if((x!=0||y!=0)&&(x!=s-1||y!=s-1)&&maze[x][y].getState()==0){
	            maze[x][y].setState(1);
	            wallCount--;
	        }
    	}
		maze[0][0].setState(0);
		maze[s-1][s-1].setState(0);
	}
	bool findPath(int x, int y, List* path, vector<vector<bool>>& visited){
		if(x<0||y<0||x>SIZE-1||y>SIZE-1||visited[x][y]||maze[x][y].getState()==1) return false;
		visited[x][y]=true;
		path->addElement(&maze[x][y]);
		if(x==SIZE-1&&y==SIZE-1) return true;
		if(findPath(x+1, y, path, visited)||findPath(x, y+1, path, visited)||findPath(x-1, y, path, visited)||findPath(x, y-1, path, visited)) return true;
		path->removeElement();
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
		vector<vector<bool>> visited(SIZE, vector<bool>(SIZE, false));
		if(findPath(0, 0, path, visited)){
			return path;
		}
		return new List();
	}
	void printMaze(){
		Grid *j=maze[0], *k;
		while(j!=NULL){
			k=j;
			while(k!=NULL){
				cout<<k->getState();
				k=k->getDir(RIGHT); 
			}
			cout<<endl;
			j=j->getDir(DOWN);
		}
	}
private:
	Grid** maze;
};

int main(){
	Maze *maze=new Maze();
	maze->printMaze();
	maze->getPath()->printPath();
}

