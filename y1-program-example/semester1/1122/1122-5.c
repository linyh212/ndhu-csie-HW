//Maze

#include <stdio.h>

int main(){
	int map[100][100];
	int size, i = 1, x, y, j, k;
	scanf("%d", &size);
	y = 0;
	x = size / 2;
	while(1){
		map[y][x] = i;
		i ++;
		if(i > size * size)  break;
		if(map[(y - 1 + size) % size][(x + 1) % size] != 0){
			y = (y + 1) % size;
		}
		else{
			y = (y - 1 + size) % size;
			x = (x + 1) % size;
		}
	}
	for(j = 0;j < size;j ++, printf("\n"))
		for(k = 0;k < size;k ++)
			printf("%4d", map[j][k]);
}
