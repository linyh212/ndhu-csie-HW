#include <stdio.h>

struct Point {
	float x, y;
};
struct Rect {
	struct Point LL, UR;
};

int main(){
	struct Rect r = {{1, 1}, {2, 2}};
	float area = (r.UR.x - r.LL.x) * (r.UR.y - r.LL.y);
	printf("%f", area);
}
