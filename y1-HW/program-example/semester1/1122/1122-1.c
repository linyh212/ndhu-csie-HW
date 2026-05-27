//Fulfilling 2D array

#include <stdio.h>

int main(){
	int array2D[3][4] = {};
	int j, k;
	for(j = 0;j < 3;j ++){
		for(k = 0;k < 4;k ++){
			array2D[j][k] = j * 4 + k + 1;
		}
	}
	for(j = 0;j < 3;j ++){
		for(k = 0;k < 4;k ++){
			printf("%d ", array2D[j][k]);
		}
		printf("\n");
	}
	for(j = 0;j < 3 * 4;j ++){
		printf("%d ", array2D[0][j]);
	}
}
