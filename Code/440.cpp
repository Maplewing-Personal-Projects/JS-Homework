/*******************************************************/
/* UVa 440 Eeny Meeny Moo                              */
/* Author: LanyiKnight [at] knightzone.org             */
/* Version: 2016/04/20                                 */
/*******************************************************/
#include <iostream>
#include <cstdio>
using namespace std;

bool isLast2(int n, int m){
    bool city[200] = {0};
    int now = 1;
    for( int count = 0, next = 0 ; count < n ; ++count, next = m ){
      if( now == 2 ) return false;

      while( next != 0 ){
        now = (now + 1) % n;
        if( !city[now] ){
          --next;
        }
      }

      city[now] = true;
    }

    if( now == 2 ) return true;
    else return false;
}

int main(){
  int m[200] = {0};
  for( int n = 3 ; n <= 150 ; ++n ){
    int guessM = 2;
    while( !isLast2(n, guessM) ){
      ++guessM;
    }

    m[n] = guessM;
  }

  int n;
  while( scanf("%d", &n) != EOF && n != 0 ){
    printf("%d\n", m[n]);
  }

  return 0;
}
