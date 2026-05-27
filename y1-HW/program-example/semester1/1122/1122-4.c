//Sorting string, compare order

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
	char str[10][10], temp[10];
	int i, j, k, len, flag;
	srand(time(NULL));
	for(j = 0;j < 10;j ++){
		len = rand() % 5 + 5;
		for(k = 0;k < len;k ++)
			str[j][k] = rand() % 26 + 'A';
		str[j][k] = '\0';
	}
	for(j = 0;j < 10;j ++)
		printf("%s\n", str[j]);
	do{
		k = 0;
		for(j = 0;j < 9;j ++){
			flag = 0;
			for(i= 0;str[j][i] != '\0' && str[j + 1][i] != '\0';i ++){
					if(str[j][i] == str[j + 1][i])	continue;
					if(str[j][i] > str[j + 1][i])	flag= 1;
					break;
			}
			if(flag == 0 && str[j + 1][i] == '\0')	flag = 1;
			if(flag == 1){
				for(i = 0;i < 10;i ++)
					temp[i] = str[j][i];
				for(i = 0;i < 10;i ++)
					str[j][i] = str[j + 1][i];
				for(i = 0;i < 10;i ++)
					str[j + 1][i] = temp[i];
				k = 1;
			}
		}
	}while(k == 1);
	printf("---------------------\n");
	for(j = 0;j < 10;j ++)
		printf("%s\n", str[j]);
}
