#include <bits/stdc++.h>
using namespace std;

class LIS{
public:
    LIS(int* arr, int size){
        length(arr, size);
    }
    void length(int* arr, int n){
        int ans=0;
        int* LIS=new int[n];
        int* LDS=new int[n];
        for(int i=n-1; i>=0; i--){
            LIS[i]=1;
            LDS[i]=1;
            for(int j=n-1; j>i; j--){
                if(arr[i]>arr[j]) LIS[i]=max(LIS[i], LIS[j]+1);
                if(arr[i]<arr[j]) LDS[i]=max(LDS[i], LDS[j]+1);
            }
            ans=max(ans, LIS[i]+LDS[i]-1);
        }
        cout<<ans<<endl;
        delete[] LIS;
        delete[] LDS;
    }
};

int main(){
    int n;
    cin>>n;
    for(int i=0; i<n; i++){
        int t;
        cin>>t;
        int* arr=new int[t];
        for(int j=0; j<t; j++){
            cin>>arr[j];
        }
        LIS lis(arr, t);
        delete[] arr;
    }
    return 0;
}

