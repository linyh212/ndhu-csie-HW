//Magic square

#include <stdio.h>

int main(){
	int arr[10][10];
  	int j, k, i, temp;
  	for(j = 0;j < 10;j ++){
		for(k = 0;k < 10;k ++){
			arr[j][k] = j * 10 + k + 1;
		}
	}
 	for(j = 0;j < 10;j ++, printf("\n")){
		for(k = 0;k < 10;k ++){
			printf("%4d", arr[j][k]);
		}
	}
	arr[0][0] = 0;
	i = 0;
	while(1){
		for(;i < 100;i ++){
			if(arr[i / 10][i % 10] != 0) break;
		}
		if(i == 100) break;
		temp = arr[i / 10][i % 10];
		for(j = i + 1;j < 100;j ++){
			if(arr[j / 10][j % 10] % temp == 0) arr[j / 10][j % 10] = 0;
		}
		i ++;
	}
	for(j = 0;j < 10;j ++, printf("\n")){
		for(k = 0;k < 10;k ++){
			printf("%4d", arr[j][k]);
		}
	}
}
