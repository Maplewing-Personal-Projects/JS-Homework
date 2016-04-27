/*******************************************************/
/* UVa 408 Uniform Generator                           */
/* Author: LanyiKnight [at] knightzone.org             */
/* Version: 2016/04/20                                 */
/*******************************************************/
#include <iostream>
#include <cstdio>
using namespace std;

int main(){
  int step, mod;
  while( scanf("%d%d", &step, &mod) != EOF ){
    bool hasUsed[1000005] = {false};
    int count = 0;
    for( int i = 0; !hasUsed[i] ; i = (i + step) % mod, ++count ){
      hasUsed[i] = true;
    }

    printf("%10d%10d    ", step, mod);
    if( count == mod ){
      printf("Good Choice");
    }
    else{
      printf("Bad Choice");
    }
    printf("\n\n");

  }
  return 0;
}
