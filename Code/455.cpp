/*******************************************************/
/* UVa 455 Periodic Strings                            */
/* Author: LanyiKnight [at] knightzone.org             */
/* Version: 2016/04/20                                 */
/*******************************************************/
#include <iostream>
#include <cstdio>
#include <string>
using namespace std;

bool isPeriod(const string &periodString, int period){
  for( int i = 0 ; i < period ; ++i ){
    for( int j = i+period ; j < periodString.length() ; j += period ){
      if( periodString[i] != periodString[j] ){
        return false;
      }
    }
  }

  return true;
}

int main(){
  int N;
  while( scanf("%d", &N) != EOF ){
    string periodString;
    for( int caseNumber = 0 ; caseNumber < N ; ++caseNumber ){
      if( caseNumber > 0 ){
        printf("\n");
      }
      cin >> periodString;

      for( int period = 1 ; period <= periodString.length() ; ++period ){
        if( periodString.length() % period == 0 && isPeriod(periodString, period) ){
          printf("%d\n", period);
          break;
        }
      }
    }
  }

  return 0;
}
