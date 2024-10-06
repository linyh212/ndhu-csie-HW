//Sorting string

#include <stdio.h>

int main() {
	int array3D[4][3][2] = {};
	int j, k, i;
	for(j = 0;j < 4;j = j + 1){
		for(k = 0;k < 3;k = k + 1){
			for(i = 0;i < 2;i = i + 1){
				array3D[j][k][i] = 2 * 3 * j + 2 * k + i + 1;
			}
		}
	}
	for(j = 0;j < 4;j = j + 1) {
		for(k = 0;k < 3;k = k + 1) {
			for(i = 0;i < 2;i = i + 1) {
				printf("%d ", array3D[j][k][i]);
			}
			printf("\n");
		}
		printf("\n");
	}
}
