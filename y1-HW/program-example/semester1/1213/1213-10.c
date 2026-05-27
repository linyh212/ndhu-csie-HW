#include <stdio.h>

char *toBinary(int d, char *result) {
  int i = 0;
  while(d) {
    result[i] = (d % 2) + '0';
    i ++;
    d = d / 2;
  }
  result[i] = '\0';
  i --;
  int j = 0;
  while(j < i)
  {
    char temp = result[i];
    result[i] = result[j];
    result[j] = temp;
    i --; 
    j ++;
  }
  return result;
}

int main()
{
  char str[1024];
  int j;
  for(j = 1;j < 10000;j ++)
    printf("%d, %s\n", j, toBinary(j, str));
}