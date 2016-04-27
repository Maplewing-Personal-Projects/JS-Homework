/*******************************************************/
/* UVa 11677 Alarm Clock                               */
/* Author: LanyiKnight [at] knightzone.org             */
/* Version: 2016/04/20                                 */
/*******************************************************/
#include <iostream>
#include <cstdio>
using namespace std;

int main(){
  int H1, M1, H2, M2;
  while( scanf("%d%d%d%d", &H1, &M1, &H2, &M2) != EOF &&
         (H1 != 0 || M1 != 0 || H2 != 0 || M2 != 0 )){
    int minutes = (H2 - H1) * 60 + (M2 - M1);
    if( minutes < 0 ){
      minutes += 24 * 60;
    }

    printf("%d\n", minutes);
  }
  return 0;
}
