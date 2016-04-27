/*******************************************************/
/* UVa 10361 Automatic Poetry                          */
/* Author: LanyiKnight [at] knightzone.org             */
/* Version: 2016/04/20                                 */
/*******************************************************/
#include <iostream>
#include <cstdio>
#include <string>
using namespace std;

int main(){
  int n;
  while( scanf("%d", &n) != EOF ){
    getchar();

    for( int caseNumber = 0 ; caseNumber < n ; ++caseNumber ){
      string s[5];
      for( int i = 0 ; i < 5 ; ++i ){
        char c;
        while( (c = getchar()) && c != '<' && c != '>' && c != '\n' ){
          s[i] += c;
        }
      }

      for( int i = 0 ; i < 5 ; ++i ){
        printf("%s", s[i].c_str());
      }
      printf("\n");

      string c2;
      getline(cin, c2);
      c2 = c2.substr(0, c2.length() - 3) + s[3] + s[2] + s[1] + s[4];
      printf("%s\n", c2.c_str());
    }
  }

  return 0;
}
