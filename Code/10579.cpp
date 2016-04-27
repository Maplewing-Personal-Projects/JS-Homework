/*******************************************************/
/* UVa 10579 Fibonacci Numbers                         */
/* Author: LanyiKnight [at] knightzone.org             */
/* Version: 2016/04/20                                 */
/*******************************************************/
#include <iostream>
#include <cstdio>
#include <vector>
using namespace std;

int main(){
  vector< vector<int> > dp( 3, vector<int>(1, 0) );
  dp[1][0] = 1;
  dp[2][0] = 1;

  int n;
  while( scanf("%d", &n) != EOF ){
    if( n >= dp.size() ){
      for( int i = dp.size() ; i <= n ; ++i ){
        dp.push_back( vector<int>(dp[i-1].size(), 0) );
        for( int j = 0 ; j < dp[i-2].size() ; ++j ){
          dp[i][j] += dp[i-1][j] + dp[i-2][j];
          if( j + 1 >= dp[i].size() && dp[i][j] / 10 > 0){
            dp[i].push_back(dp[i][j] / 10);
          }
          else if( dp[i][j] / 10 ){
            dp[i][j+1] += dp[i][j] / 10;
          }
          dp[i][j] %= 10;
        }
        if( dp[i-1].size() > dp[i-2].size() ){
          for( int j = dp[i-2].size() ; j < dp[i-1].size() ; ++j ){
            dp[i][j] += dp[i-1][j];
            if( j + 1 >= dp[i].size() && dp[i][j] / 10 > 0){
              dp[i].push_back(dp[i][j] / 10);
            }
            else if( dp[i][j] / 10 ){
              dp[i][j+1] += dp[i][j] / 10;
            }
            dp[i][j] %= 10;
          }
        }
      }
    }

    for( int i = dp[n].size() - 1 ; i >= 0 ; --i ){
      printf("%d", dp[n][i]);
    }
    printf("\n");
  }
  return 0;
}
