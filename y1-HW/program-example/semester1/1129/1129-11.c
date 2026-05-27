#include <stdio.h>
#include <stdlib.h>

int main(){
	int *p = malloc(sizeof(int) * 10);
	int j;
	for(j = 0;j < 10;j ++)
		p[j] = j + 1;
	for(j = 0;j < 10;j ++)
		printf("%d ", *(p + j));
  //for(j = 0;j < 10;j ++)
  //  printf("%d ", p[j]);
  //int *p1 = malloc(sizeof(int)), *p2 = malloc(sizeof(int));
  /*scanf("%d%d", p1, p2);
  if(*p1 > *p2)
    printf("%d\n", *p1);
  else
    printf("%d\n", *p2);*/
}
