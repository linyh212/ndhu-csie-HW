#include <stdio.h>
#include <stdlib.h>

typedef struct {
	float x, y;
} Point;

typedef struct {
	Point point;
	float slope;
} Line;

int main(){
	Line line[100];
	Point dot[100];
	char c;
	scanf("%f%c %f%c %f", &(line[0].point.x), &c, &(line[0].point.y), &c, &(line[0].slope));
	float b=0;
	b=(line[0].point.y)-(line[0].slope)*(line[0].point.x);
	scanf("%f%c %f", &(dot[0].x), &c, &(dot[0].y));
	if((dot[0].y)==(line[0].slope)*(dot[0].x)+b) printf("Yes");
	else printf("No");
}
