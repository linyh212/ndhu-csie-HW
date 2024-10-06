#include <stdio.h>

int main(){
	int n, array[100], i=0;
	scanf("%d", &n);
	while(n>0){
		array[i] = n%2;
		n/=2;
		i++;
	}
	for(int j=i-1; j>=0; j--){
		printf("%d",array[j]);
	}
}
