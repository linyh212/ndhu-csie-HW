#include <iostream>
using namespace std;

#define SIZE 4

class Sudoku{
private:
	int board[SIZE][SIZE];
public:
	Sudoku(int b[SIZE][SIZE]){
		for(int i=0; i<SIZE; i++){
			for(int j=0; j<SIZE; j++){
				board[i][j]=b[i][j];
			}
		}
	}
	void displayBoard() const{
		for(int i=0; i<SIZE; i++){
			for(int j=0; j<SIZE; j++){
				cout<<board[i][j]<<" ";
			}
			cout<<endl;
		}
	}
	bool isAns(int row, int col, int num) const{
		for(int i=0; i<SIZE; i++){
			if(board[row][i]==num||board[i][col]==num) return false;
		}
		int startRow=row-row%2, startCol=col-col%2;
		for(int i=0; i<2; i++){
			for(int j=0; j<2; j++){
				if(board[i+startRow][j+startCol]==num) return false;
			}
		}
		return true;
	}
	bool findEmpty(int &row, int &col) const{
		for(row=0; row<SIZE; row++)
            for(col=0; col<SIZE; col++)
                if(board[row][col]==0) return true;
        return false;
	}
	bool solveSudoku(){
		int row, col;
		if(!findEmpty(row, col)) return true;
		for(int num=1; num<SIZE+1; num++){
			if(isAns(row, col, num)){
				board[row][col]=num;
				displayBoard();
				if(solveSudoku()) return true;
				board[row][col]=0;
			}
		}
		return false;
	}
};

int main(){
    int b[SIZE][SIZE];
    for(int i=0; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			cin>>b[i][j];
		}
	}
    Sudoku game(b);
    game.solveSudoku();
    return 0;
}
