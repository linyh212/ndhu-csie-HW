#include <stdio.h>

int main(){
	int j, k, l, m;
	scanf("%d %d %d %d", &j, &k, &l, &m);
	if(j>k&&l>m&&k>=l||j>k&&m>l&&k>=m||k>j&&l>m&&j>=l||k>j&&m>l&&j>=m||l>m&&j>k&&m<=j||m>l&&j>k&&l>=j||l>m&&k>j&&m>=k||m>l&&k>j&&l>=k) printf("no overlay");
	else printf("overlay");
}
