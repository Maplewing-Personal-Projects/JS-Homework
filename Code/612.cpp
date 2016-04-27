/*******************************************************/
/* UVa 612 DNA Sorting                                 */
/* Author: LanyiKnight [at] knightzone.org             */
/* Version: 2016/04/19                                 */
/*******************************************************/
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct DNA{
  string sequence;
  int inverse;
  int id;

  DNA(string s, int inputID){
    sequence = s;
    inverse = 0;
    id = inputID;

    int count[26] = {0};
    for( int i = 0 ; i < s.length() ; ++i ){
      for( int j = s[i] - 'A' + 1 ; j < 26 ; ++j ){
        inverse += count[j];
      }

      ++count[s[i]-'A'];
    }
  }
};

bool cmpDNA( const DNA &a, const DNA &b ){
  return (a.inverse < b.inverse) || (a.inverse == b.inverse && a.id < b.id);
}

int main(){
  int M;
  while( scanf("%d", &M) != EOF ){
    for( int caseNumber = 0 ; caseNumber < M ; ++caseNumber ){
      if( caseNumber > 0 ){
        printf("\n");
      }

      int n, m;
      scanf("%d%d", &n, &m);

      vector<DNA> DNAs;
      for( int i = 0 ; i < m ; ++i ){
        string input;
        cin >> input;

        DNA dna(input, i);
        DNAs.push_back(dna);
      }

      sort(DNAs.begin(), DNAs.end(), cmpDNA);

      for( int i = 0 ; i < DNAs.size() ; ++i ){
        printf("%s\n", DNAs[i].sequence.c_str());
      }

    }
  }

  return 0;
}