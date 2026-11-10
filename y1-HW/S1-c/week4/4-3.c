#include <stdio.h>

int main(){
	int n, i, a=0;
	float sum=0;
	scanf("%d" ,&n);
	int array[n];
	for(i=0; i<n; i++){
		array[n]=0;
		scanf("%d", &array[i]);
		sum +=array[i];
	}
	sum/=n;
	for(i=0; i<n; i++){
		if(array[i]<sum) a+=1;
	}
	printf("%d", a);
	return 0;
}
