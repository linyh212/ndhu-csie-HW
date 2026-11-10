#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct point{
    double x;
    double y;
};

typedef struct point point;

point create_point(double x, double y){
    point p;
    p.x=x;
    p.y=y;
    return p;
}

int compare(const void *a, const void *b){
    return ((point *)b)->x-((point *)a)->x;
}

double length(point a, point b){
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

int main(){
	int b, n;
	point arr[101];
	while(scanf("%d",&b)!=EOF){
		double high, total_length, m, x;
		for(int i=0; i<b; i++){
			scanf("%d",&n);
			for(int j=0; j<n; j++){
				scanf("%lf %lf", &(arr[j].x), &(arr[j].y));
			}
			qsort(arr, n, sizeof(point), compare);
			high=0, total_length=0;
			for(int j=1; j<n; j++){
				if(arr[j].y>high){
					m=(arr[j].y-arr[j-1].y)/(arr[j].x-arr[j-1].x);
					x=(high-(arr[j].y - m * arr[j].x))/m;
					total_length+=length(arr[j], create_point(x, high));
					high=arr[j].y;
				}
			}
			printf("%.2lf\n", total_length);
		}
	}
}
