#include <stdio.h>

int main(){
	int a,b,c,d,temp;
	scanf("%d%d%d%d", &a,&b,&c,&d);
	if(b>=a&&b>=c&&b>=d){
		temp=a;
		a=b;
		b=temp;
	}
	if(c>=a&&c>=b&&c>=d){
		temp=a;
		a=c;
		c=temp;
	}
	if(d>=a&&d>=a&&d>=b&&d>=c){
		temp=a;
		a=d;
		d=temp;
	}
	printf("%d", a);
	return 0;
}
