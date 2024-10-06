#include <stdio.h>

int toBinary(int d){
	int base = 1;
	while(base <= d)
		base *= 2;
	base /=2;
	int ans = 0;
	while(base){
		ans = ans * 10 + d / base;
		d %= base;
		base /= 2;
	}
	return ans;
}

int main(){
	int n;
	scanf("%d", &n);
	printf("%d", toBinary(n));
}
