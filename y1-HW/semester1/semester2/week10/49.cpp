#include <bits/stdc++.h>
using namespace std;

class EmirpChecker{
public:
    EmirpChecker(){}
    bool isPrime(int N){
        if(N<=1) return false;
        for(int a=2; a<=sqrt(N); a++){
            if(N%a==0) return false;
        }
        return true;
    }
    int reverse(int M){
        int a[100];
        int i=0;
        int total=0;
        while(M!=0){
            a[i]=M%10;
            M=M/10;
            i++;
        }
        for(int j=0; j<i; j++){
            total=total*10+a[j];
        }
        return total;
    }
    bool isEmirp(int K){
        if(!isPalindrome(K)||!isPrime(K)||!isPrime(reverse(K))) return false;
        return true;
    }
private:
    bool isPalindrome(int num){
        int s[35];
        int i=0;
        while(num!=0){
            s[i]=num%10;
            num=num/10;
            i++;
        }
        for(int j=0, k=i-1; j<=k; j++, k--){
            if(s[j]!=s[k]) return true;
        }
        return false;
    }
};

int main(){
    int m;
    EmirpChecker checker;
    while(cin>>m){
        if(checker.isPrime(m)&&m>=10){
            if(checker.isEmirp(m)) cout<<m<<" is emirp."<<endl;
            else cout<<m<<" is prime."<<endl;
        }
		else cout<<m<<" is not prime."<<endl;
    }
    return 0;
}

