#include <stdio.h>
#include <string.h>

int main(){
	char array[10000];
	//printf("%d\n", sizeof(int));
	//printf("%p\n",array); 
	//scanf("%s", &array[0]);//&:¨ú§}operator
	while(scanf("%s",&array[0])&&!(array[0]=='0'&&strlen(array)==1)){
		int i, sum1=0, sum2=0, sum3;
		for(i=0; i<strlen(array); i+=2){
			sum1+=array[i]-'0';
		}
		for(i=1; i<strlen(array); i+=2){
			sum2 +=	array[i]-'0';
		}
		sum3=sum1-sum2;
		if(sum3%11==0) printf("%s is a multiple of 11.\n",&array[0]);
		else printf("%s is not a multiple of 11.\n",&array[0]);
	}
}
