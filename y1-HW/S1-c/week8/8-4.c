#include <stdio.h>

struct Course{
    int sid;
    int student_id;
    int course_id;
};
struct Score{
    int sid;
    int score;
};

int main(){
	int m, n;
	while(scanf("%d %d", &m, &n)==2){
		struct Course courses[1000];
		struct Score scores[1000];
		for (int i=0; i<m; i++){
			scanf("%d %d %d", &courses[i].sid, &courses[i].student_id, &courses[i].course_id);
		}
		int total=0;
		for(int i=0; i<m; i++){
			int student_id=courses[i].student_id;
			int already_calculated=0;
			for(int j=0; j<i; j++){
				if(courses[j].student_id==student_id){
					already_calculated=1;
					break;
				}
			}
			if(!already_calculated) total++;
		}
		for(int i=0; i<n; i++){
			scanf("%d %d", &scores[i].sid, &scores[i].score);
		}
		int fail=0;
		for(int i=0; i<=n; i++){
			int student_id=courses[i].student_id;
			int has_failed=0;
			int already_checked=0;
			for(int j=0; j<i; j++){
				if(courses[j].student_id==student_id){
					already_checked=1;
					break;
				}
			}
			if(!already_checked){
				for(int j=0; j<n; j++){
					if(scores[j].sid==courses[i].sid&&scores[j].score<60){
						has_failed=1;
						break;
					}
				}
				if(has_failed) fail++;
			}
		}
		printf("%d %d\n", total, fail);
	}
	return 0;
}
