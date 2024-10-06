#include <stdio.h>
#include <stdlib.h>

struct Data {
	int num;
	struct Data *p;
};

int main(){
	int input;
	struct Data *head = NULL;
	while(1){
		scanf("%d", &input);
		if(input == 0) break;
		struct Data *temp = malloc(sizeof(struct Data));
		temp->num = input;
		temp->p = NULL;
		if(head == NULL) head = temp;
		else{
			temp->p = head;
			head = temp;
			/*struct Data *j = head;
   			while(j->p != NULL)
        			j = j->p;
      				j->p = temp;*/
		}
	}
	struct Data *j = head;
	while(j != NULL){
		printf("%d ", j->num);
		j = j->p;
	}
	printf("\n");
	while(head != NULL){
		struct Data *j = head;
		head = head->p;
		j->p = NULL;
		printf("%d ", j->num);
	}
}
