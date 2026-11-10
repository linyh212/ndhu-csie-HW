#include <iostream>
#include <stack>
using namespace std;

struct p{
    int x, y;
};
void maze(int m){
	for(int k=0; k<m; k++){
        int a, b;
        cin>>a>>b;
        int m_x, m_y;
        cin>>m_x>>m_y;
        int draw[a+1][b*2+1]={0};
        for(int i=0; i<a+1; i++){
            for(int j=0; j<b*2+1; j++){
                if(i!=a){
                    if(j&1) draw[i][j]=2;
                    else draw[i][j]=1;
                }
                else if(j&1) draw[i][j]=2;
            }
        }
        stack<p> m_k;
        while(!m_k.empty()) m_k.pop();
        p temp;
        temp.x=m_x-1;
        temp.y=m_y*2-1;
        m_k.push(temp);
        for(int count=a*b-1; count>0; count--){
            char c;
            cin>>c;
            temp=m_k.top();
            if(c=='U'){
                temp.x++;
                draw[temp.x][temp.y]=0;
                m_k.push(temp);
            }
            if(c=='D'){
                draw[temp.x][temp.y]=0;
                temp.x--;
                m_k.push(temp);
            }
            if(c=='L'){
                temp.y-=2;
                draw[temp.x][temp.y+1]=0;
                m_k.push(temp);
            }
            if(c=='R'){
                temp.y+=2;
                draw[temp.x][temp.y-1]=0;
                m_k.push(temp);
            }
            if(c=='F'){
                int num;
                cin>>num;
                for(int i=0; i<num; i++) m_k.pop();
                count++;
                temp=m_k.top();
            }
        }
        for(int i=a; i>=0; i--){
            for(int j=0; j<b*2+1; j++){
                if(i==a&&j==b*2) continue;
                if(draw[i][j]==1) cout<<"|";
                if(draw[i][j]==2) cout<<"_";
                if(draw[i][j]==0) cout<<" ";
            }
            cout<<endl;
        }
        if(k!=m-1) cout<<endl;
    }	
}

int main(){
    int n;
    cin>>n;
	maze(n); 
}
