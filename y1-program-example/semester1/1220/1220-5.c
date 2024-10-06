#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	if(argc == 2){
		FILE *in = fopen(argv[1], "r");
		char ch;
		while(fscanf(in, "%c", &ch) != EOF)
			printf("%c", ch);
		fclose(in);
	}
	system("pause");
}
