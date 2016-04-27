#include <iostream>
#include <cstdio>
#include <sstream>
#include <vector>
using namespace std;

int main(){
  int N;
  while( scanf("%d ", &N) != EOF && N != 0 ){
    vector<bool> places(N+1, false);

    int count = 0;
    string line;
    while( getline(cin, line) && line != "0" ){
      stringstream ss(line);
      int place;
      bool usefulLine = false;
      while( ss >> place ){
        if( !places[place] ){
          usefulLine = true;
          places[place] = true;
        }
      }

      if( usefulLine ){
        ++count;
      }
    }

    printf("%d\n", count);
  }
  return 0;
}
