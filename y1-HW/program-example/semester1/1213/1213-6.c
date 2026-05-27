#include <stdio.h>

int isA(int n){
	int temp = n, sum = 0, d;
	while(temp){
		d = temp % 10;
		sum += d * d * d;
		temp /= 10;
	}
	return sum == n;
}

int isP(int n){
	int sum = 0, j;
	for(j = 1;j < n;j ++)
		if(n % j == 0) sum += j;
	return sum == n;
}

int main(){
	int j;
	for(j = 1;j < 10000;j ++)
		if(isP(j)) printf("%d ", j);
}
