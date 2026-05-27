#include <iostream>
#include <cstdio>

int main()
{
  //freopen("output2.txt", "r", stdin);
  FILE *in = fopen("output2.txt", "r");
  char str[1024];
  while(fgets(str, 1024, in))
    std::cout << str;
}