#include <stdio.h>

void foo(int *n){
	(*n) ++;
	printf("foo: %d\n", *n);
}

int main(){
	int a = 10;
	foo(&a);
	printf("%d", a);
}
