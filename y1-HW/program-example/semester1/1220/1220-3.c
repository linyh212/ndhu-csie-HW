#include <stdio.h>

int main(){
	FILE *file;
	file = fopen("test.txt", "w+");
	fprintf(file, " some text......");
	fclose(file);
}
