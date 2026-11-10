#include <stdio.h>

int main(){
	int i, j, a, b;
	while(scanf("%d %d\n", &i, &j)!=EOF){
		int max=0;
		a=i;
		b=j;
		if(i<j){
			a=j;
			b=i;
		}
		for(int k=b; k<=a; k++){
			int count=1;
			int num=k;
			while(num>1){
				if(num%2==0) num/=2;
				else num=num*3+1;
				count++;
			}
			if(count>=max) max=count;
		}
		printf("%d %d %d\n", i, j, max);
	}
}	
