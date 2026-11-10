#include <stdio.h>
#include <math.h>

int main(){
	int n, i;
	double u = 0, red = 0, sum = 0, sigma = 0;
	scanf("%d", &n);
	int array[n];
	for(i=0; i<n; i++){
		array[n]=0;
		scanf("%d", &array[i]);
		u+=array[i];
	}
	u/=n;
	for(i=0; i<n; i++){
		red=array[i]-u;
		red*=red;
		sum+=red;
	}
	sum/=n;
	sigma=sqrt(sum);
	printf("%.2f", sigma);
	return 0;
}
