#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char studentID[20];
	int programming, programmingLab, calculus, sum;
} Student;

int compare(const void *a, const void *b){
	return ((Student *)b)->sum-((Student *)a)->sum;
}

int main(){
	int n;
	Student grade[100];
	scanf("%d", &n);
	for(int i=0; i<n; i++){
		scanf("%s %d %d %d", &(grade[i].studentID), &(grade[i].programming), &(grade[i].programmingLab), &(grade[i].calculus));
		grade[i].sum=grade[i].programming+grade[i].programmingLab+grade[i].calculus;
	}
	qsort(grade, n, sizeof(Student), compare);
	for(int i=0; i<3; i++){
		printf("%s\n", grade[i].studentID);
	} 	
}
