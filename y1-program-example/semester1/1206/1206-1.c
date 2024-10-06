#include <stdio.h>

struct Person {
	char name[50];
  	int age;
  	float height;
};

int main(){
	struct Person jack, *p;
	jack.age = 18;
	p = &jack;
	scanf("%s", (*p).name);
	printf("%s", p->name);
}
