#include <stdio.h>

int main(){
	int arr[100000], l, h, r;
	int max_r=0;
	while(scanf("%d %d %d",&l,&h,&r)==3){
		if(r>max_r) max_r=r;
		for (int i=l; i<=r; i++){
			if(h>arr[i*2]) arr[i*2]=h;
			if(i<r&&h>arr[i*2+1]) arr[i*2+1]=h;
		}
	}
	int p=0;
	for(int i=0; i<max_r; i++){
		if(arr[i*2]!=p){
			printf("%d %d ", i, arr[i*2]);
			p=arr[i*2];
		}
		if(arr[i*2+1]!=p){
			printf("%d %d ", i, arr[i*2+1]);
			p=arr[i*2+1];
		}
	}
	printf("%d %d", max_r, 0);
}
