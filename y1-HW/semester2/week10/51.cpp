#include <bits/stdc++.h>
using namespace std;

class SquareChecker{
private:
    vector<vector<char>> matrix;
    int m, n;
public:
    SquareChecker(int rows, int cols, const vector<vector<char>>& grid): m(rows), n(cols), matrix(grid){}
    int check(int row, int col){
        char mid=matrix[row][col];
        int length=1;
        int index=1;
        while(true){
            int newLength=length+2;
            int topLeftRow=row-index;
            int topLeftCol=col-index;
            if(!(0<=topLeftRow&&topLeftRow<m&&0<=topLeftCol&&topLeftCol<n&&topLeftRow+newLength-1<m&&topLeftCol+newLength-1<n)){
                return length;
            }
            for(int i=0; i<newLength; i++){
                if(!(matrix[topLeftRow][topLeftCol+i]==mid&&matrix[topLeftRow+newLength-1][topLeftCol+i]==mid&&matrix[topLeftRow+i][topLeftCol]==mid&&matrix[topLeftRow+i][topLeftCol+newLength-1]==mid)){
                    return length;
                }
            }
            length=newLength;
            index+=1;
        }
    }
};

int main() {
    int T;
    cin>>T;
    for(int t=0; t<T; t++){
        int m, n, q;
        cin>>m>>n>>q;
        vector<vector<char>> matrix(m, vector<char>(n));
        for(int i=0; i<m; i++){
            for(int j=0; j<n; j++){
                cin>>matrix[i][j];
            }
        }
        SquareChecker squareChecker(m, n, matrix);
        cout<<m<<" "<<n<<" "<<q<<"\n";
        for(int i=0; i<q; i++){
            int row, col;
            cin>>row>>col;
            cout<<squareChecker.check(row, col)<<"\n";
        }
    }
    return 0;
}

