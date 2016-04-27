/*******************************************************/
/* UVa 11292 Dragon of Loowater                        */
/* Author: LanyiKnight [at] knightzone.org             */
/* Version: 2016/04/20                                 */
/*******************************************************/
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

int main(){
  int n, m;
  while( scanf("%d%d", &n, &m) != EOF && n != 0 && m != 0 ){
    vector<int> dragonHeads(n);
    vector<int> knightHeights(m);

    for( int i = 0 ; i < n ; ++i ){
      scanf("%d", &dragonHeads[i]);
    }

    for( int i = 0 ; i < m ; ++i ){
      scanf("%d", &knightHeights[i]);
    }

    sort(dragonHeads.begin(), dragonHeads.end());
    sort(knightHeights.begin(), knightHeights.end());

    int cost = 0;
    int i, j;
    for( i = 0, j = 0 ;
         i < dragonHeads.size() && j < knightHeights.size() ; ){
      if( dragonHeads[i] <= knightHeights[j] ){
        cost += knightHeights[j];
        ++i;
        ++j;
      }
      else {
        ++j;
      }
    }

    if( i != dragonHeads.size() ){
      printf("Loowater is doomed!\n");
    }
    else{
      printf("%d\n", cost);
    }

  }
  return 0;
}
