/*******************************************************/
/* UVa 558 Wormholes                                   */
/* Author: LanyiKnight [at] knightzone.org             */
/* Version: 2016/04/20                                 */
/*******************************************************/
#include <iostream>
#include <cstdio>
using namespace std;

const int MAX_DISTANCE = 20000000;

struct Edge{
  int from;
  int to;
  int distance;
};

int main(){
  int c;
  while ( scanf("%d", &c) != EOF ) {
    for( int caseNumber = 0 ; caseNumber < c ; ++caseNumber ){
      int n, m;
      scanf("%d%d", &n, &m);

      Edge edges[2005];
      for( int i = 0 ; i < m ; ++i ){
        scanf("%d%d%d", &(edges[i].from), &(edges[i].to), &(edges[i].distance));
      }

      int fromSourceDistance[1005] = {0};
      for( int i = 1 ; i < n ; ++i ){
        fromSourceDistance[i] = MAX_DISTANCE;
      }

      // Bellman-Ford Algorithm
      for( int i = 0 ; i < n-1 ; ++i ){
        for( int j = 0 ; j < m ; ++j ){
          fromSourceDistance[edges[j].to] = min (
            fromSourceDistance[edges[j].from] + edges[j].distance,
            fromSourceDistance[edges[j].to] );
        }
      }

      // one more to find negative cycle
      bool hasNegativeCycle = false;
      for( int j = 0 ; j < m ; ++j ){
        if( fromSourceDistance[edges[j].from] + edges[j].distance < fromSourceDistance[edges[j].to] ){
          printf("possible\n");
          hasNegativeCycle = true;
          break;
        }
      }

      if( !hasNegativeCycle ){
        printf("not possible\n");
      }

    }
  }

  return 0;
}
