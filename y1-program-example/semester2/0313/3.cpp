#include <cstring>
#include <iostream>
#include <string>
using namespace std;

int main()
{
  /*int j;
  char str[1024];
  scanf("%s", str);
  //for(j = 0;str[j] != '\0';j ++);
  j = strlen(str);
  printf("%d",j);*/

  /*string str;
  cin >> str;
  cout << str.size();*/

  /*int j, k;
  char s1[1024], s2[1024];
  scanf("%s%s", s1, s2);
  //for(j = 0;s1[j] != '\0';j ++);
  //for(k = 0;s2[k] != '\0';k ++) {
  //  s1[j + k] = s2[k];
  //}
  s1[j + k] = '\0';
  strcat(s1, s2);
  printf("%s", s1);*/

  /*string s1, s2, s3;
  cin >> s1 >> s2;
  s3 = s1 + s2;
  s1 += s2;
  cout << s3;*/

  /*int j, k, p = -1;
  
  /*char str[1024], sub[1024];
  scanf("%s%s", str, sub);
  //for(j = 0;str[j] != '\0';j ++) {
  //  for(k = 0;sub[k] != '\0';k ++)
  //    if(str[j + k] != sub[k])
  //      break;
  //  if(sub[k] == '\0') {
  //    p = j;
  //    break;
  //  }
  //}
  //printf("%d", p);
  p = strstr(str, sub) - str;
  printf("%d", p);*/

  string s1, s2;
  cin >> s1 >> s2;
  int p = s1.find(s2);
  cout << p;

  /*int j;
  char str[1024];
  scanf("%s", str);
  for(j = 0;str[j] != '\0';j ++)
    printf("%c\n", str[j]);*/

  /*int j;
  string str;
  cin >> str;
  //for(j = 0;j < str.length();j ++)
    //cout << str[j] << endl;
    //cout << str.at(j) << endl;
  //for(auto ch = str.begin();ch != str.end();ch ++)
  //  cout << *ch << endl;
  for(auto ch : str)
    cout << ch << endl;*/

  /*int j, k;
  int index, len;
  char str[1024], sub[1024];
  scanf("%s", str);
  scanf("%d%d", &index, &len);
  //for(j = 0;j < len;j ++)
  //  sub[j] = str[index + j];
  //sub[j] = '\0';
  strncpy(sub, str + index, len);
  sub[len] = '\0';
  printf("%s", sub);*/

  /*string str, sub;
  int index, len;
  cin >> str >> index >> len;
  sub = str.substr(index, len);
  cout << sub;*/
  
}