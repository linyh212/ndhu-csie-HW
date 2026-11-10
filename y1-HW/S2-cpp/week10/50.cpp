#include <bits/stdc++.h>
using namespace std;

const int dx[]={-1, -1, -1, 0, 0, 1, 1, 1};
const int dy[]={-1, 0, 1, -1, 1, -1, 0, 1};

int main(){
    int n, m, fieldNumber=1;
    while(cin>>n>>m&&(n!=0&&m!=0)){
        vector<string> grid(n);
        for(int i=0; i<n; i++){
            cin>>grid[i];
        }
        vector<vector<int>> mineCount(n, vector<int>(m, 0));
        for(int i=0; i<n; i++){
            for(int j=0; j<m; j++){
                if(grid[i][j]=='.'){
                    int count=0;
                    for(int dir=0; dir<8; dir++){
                        int newRow=i+dx[dir];
                        int newCol=j+dy[dir];
                        if(newRow>=0&&newRow<n&&newCol>=0&&newCol<m&&grid[newRow][newCol]=='*'){
                            count++;
                        }
                    }
                    mineCount[i][j]=count;
                }
            }
        }
        if(fieldNumber>1){
            cout<<"\n";
        }
        cout<<"Field #"<<fieldNumber++ <<":\n";
        for(int i=0; i<n; i++){
            for(int j=0; j<m; j++){
                if(grid[i][j]=='*') cout<<"*";
                else cout<<mineCount[i][j];
            }
            cout<<"\n";
        }
    }
    return 0;
}

