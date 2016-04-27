/*******************************************************/
/* UVa 11777 Automate the Grades                       */
/* Author: LanyiKnight [at] knightzone.org             */
/* Version: 2016/04/20                                 */
/*******************************************************/
#include <iostream>
#include <cstdio>
using namespace std;

struct Grade{
  char name;
  int score;
};

int main(){
  const Grade gradeTable[] = { {'A', 90}, {'B', 80}, {'C', 70}, {'D', 60}, {'F', 0}};
  int gradeTableSize = sizeof(gradeTable) / sizeof(Grade);

  int T;
  while( scanf("%d", &T) != EOF ){
    int term1, term2, final, attendence, classTest1, classTest2, classTest3;
    for( int caseNumber = 1 ; caseNumber <= T ; ++caseNumber ){
      scanf("%d%d%d%d%d%d%d", &term1, &term2, &final, &attendence, &classTest1, &classTest2, &classTest3);
      int score = term1 + term2 + final + attendence +
                  ( max( max(classTest1, classTest2), classTest3 ) +
                    max( min(classTest1, classTest2), min( max(classTest1, classTest2), classTest3 ) ) ) / 2;
      for( int i = 0 ; i < gradeTableSize ; ++i ){
        if( score >= gradeTable[i].score ){
          printf("Case %d: %c\n", caseNumber, gradeTable[i].name);
          break;
        }
      }
    }
  }

  return 0;
}
