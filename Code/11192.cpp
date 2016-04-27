#include <iostream>
#include <cstdio>
using namespace std;

int main(){
  int G;
  string input;

  while( scanf("%d", &G) != EOF && G != 0 ){
    cin >> input;

    int numOfGroupMember = input.length() / G;
    for( int i = 0 ; i < input.length() ; i += numOfGroupMember ){
      for( int j = i + numOfGroupMember - 1 ; j >= i ; j-- ){
        printf("%c", input[j]);
      }
    }

    printf("\n");
  }

  return 0;
}