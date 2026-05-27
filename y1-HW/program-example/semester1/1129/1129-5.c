#include <stdio.h>

int main(){
	char before[5] = {'a', 'b', 'c', 'd', 'e'}, arr[5] = {'1', '2', '3', '4', '5'}, after[5] = {'A', 'B', 'C', 'D', 'E'};
	int j;
	int *p;
	p = arr;
  //printf("%p\n", arr);
  //for(p = arr, j = 0;j < 5;j ++)
  //  printf("%p, %c\n", p + j, *(p + j));
  //for(p = arr;p < arr + sizeof(arr) / sizeof(arr[0]);p ++)
  //  printf("%p, %c\n", p, *p);
	printf("%c\n", p[-1]);
}
