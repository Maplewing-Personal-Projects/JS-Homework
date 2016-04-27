/*******************************************************/
/* UVa 231 Testing the CATCHER                         */
/* Author: LanyiKnight [at] knightzone.org             */
/* Version: 2016/04/24                                 */
/*******************************************************/
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

int main(){
  int number;
  vector<int> LIS;
  int caseNumber = 0;
  while( scanf("%d", &number) != EOF ){
    if( number == -1 && LIS.size() == 0 ){
      break;
    }

    if( number == -1 ){
      if( caseNumber > 0 ){
        printf("\n");
      }
      printf("Test #%d:\n", ++caseNumber);
      printf("  maximum possible interceptions: %d\n", LIS.size());
      LIS.clear();
      continue;
    }

    if( LIS.size() == 0 || number <= LIS.back() ){
      LIS.push_back(number);
    }
    else{
      *upper_bound(LIS.begin(), LIS.end(), number, greater<int>()) = number;
    }



  }
  return 0;
}
