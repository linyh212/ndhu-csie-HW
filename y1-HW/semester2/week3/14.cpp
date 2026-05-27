#include <iostream>
using namespace std;

class reverse_add{
public:
    static void matrix(long long d){
        int count=0;
        while(1){
            long long rem=0, rev=0, temp=d;
            while(temp!=0){
                rem=temp%10;
                rev=rev*10+rem;
                temp/=10;
            }
            if(d!=rev||(count==0&&d==rev)){
                d+=rev;
                count++;
            }
            else break;
        }
        cout<<count<<" "<<d<<endl;
    }
};

int main(){
    int n;
    cin >> n;
    for(int i = 0; i < n; i++){
        long long d;
        cin >> d;
        reverse_add::matrix(d);
    }
}
