#include <stdio.h>

typedef struct {
	float x, y;
} Point;

int main(){
	Point p1 = {1, 2}, p2 = {2, 1};
	float d = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
	float up = d, low = 0, mid;
	while(up - low > 0.00001){
		mid = (up + low) / 2;
		if(d > mid * mid) low = mid;
		else up = mid;
	}
	printf("%f", mid);
	if(mid < 0.001) printf();
	else printf();
}
