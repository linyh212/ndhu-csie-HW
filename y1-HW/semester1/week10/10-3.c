#include <stdio.h>
#include <string.h>

typedef struct {
	char integer[1000];	
	char decimal[1000];
} DeciVar;

int main(){
	char a[2001];
	DeciVar deciVar[2];
	for(int i=0; i<2; i++){
		memset(a, 0, sizeof(a));
		scanf("%s", a);
		char *p=strtok(a, ".");
		if(p!=NULL){
			strcpy(deciVar[i].integer, p);
			p=strtok(NULL, ".");
			if(p!=NULL) strcpy(deciVar[i].decimal, p);
		}
	}
	int dec_sum[1001]={0}, int_sum[1001]={0};
	int flag=0, up=0;
	int dec_len=strlen(deciVar[0].decimal)>strlen(deciVar[1].decimal)? strlen(deciVar[0].decimal):strlen(deciVar[1].decimal);
	for(int i=strlen(deciVar[0].decimal)-1; i>=0; i--){
		dec_sum[i]=deciVar[0].decimal[i]-'0';
	}
	for(int i=strlen(deciVar[1].decimal)-1; i>=0; i--){
		dec_sum[i]+=up+(deciVar[1].decimal[i]-'0');
		up=0;
		if(dec_sum[i]>=10&&i!=0){
			dec_sum[i]-=10;
			up=1;
		}
		else if(dec_sum[i]>=10&&i==0){
			dec_sum[i]-=10;
			flag=1;
		}
	}
	int int_len=strlen(deciVar[0].integer)>strlen(deciVar[1].integer)? strlen(deciVar[0].integer):strlen(deciVar[1].integer);
	int count=0;
	if(flag) int_sum[0]++;
	for(int i=strlen(deciVar[0].integer)-1; i>=0; i--){
		int_sum[count]+=(deciVar[0].integer[i]-'0');
		count++;
	}
	count = 0;
	for(int i=strlen(deciVar[1].integer)-1; i>=0; i--){
		int_sum[count]+=(deciVar[1].integer[i]-'0');
		count++;
	}
	for(int i=0; i<int_len; i++){
		if(int_sum[i]>=10){
			int_sum[i]-=10;
			int_sum[i+1]++;
			if(i+1>=int_len)int_len++;
		}
	}
	for(int i=dec_len-1; i>=0; i--){
		if(dec_sum[i]==0) dec_len--;
		else break;
	}
	for(int i=int_len-1; i>=0; i--){
		printf("%d", int_sum[i]);
	}
	if(dec_len>0){
		printf(".");
		for(int i=0; i<dec_len; i++){
			printf("%d", dec_sum[i]);
		}
	}
	return 0;
}
