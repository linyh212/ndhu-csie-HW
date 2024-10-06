#include <stdio.h>

int main(){
	int n, k;
	while(scanf("%d %d",&n,&k)&&n!=0&&k!=0){
		if(n==1) printf("%d\n",n);
		else{
			int arr[101];
			for(int i=1; i<=n; i++){
				arr[i]=i;
			}
			int dead=0, count=1, total=n;
			while(total>1){
				int num=0;
				while(num<k){
					if(count>n) count=1;
					if(arr[count]>0) num++;
					count++;
				}
				dead=count-1;
				arr[dead]=0;
				num=0;
				while(num<k){
					if(count>n) count=1;
					if(arr[count]>0) num++;
					count++;
				}
				arr[dead]=arr[count-1];
				arr[count-1]=0;
				count=dead+1;
				total--;
			}
			if(arr[dead]==1) printf("1\n");
			else printf("%d\n", n-(arr[dead]-1)+1);
		}
	}
}
