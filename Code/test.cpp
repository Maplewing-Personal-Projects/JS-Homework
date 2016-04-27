#include <iostream>
#include <cstdio>
#include <string>
#include <cctype>
using namespace std;

struct Token{
  int type; // text = 1, number = 2, ( = 3, ) = 4;

  char text;
  long long number;

  int nextPoint;
};

// f -> text f | number f f | ( f ) f |
Token next( const string &input, int point ){
  if( input.length() == point ){
    Token t = {0, 0, 0, point};
    return t;
  }

  if( isalpha( input[point] ) ){
    Token t = {1, input[point], 0, point+1};
    return t;
  }
  else if( isdigit( input[point] ) ){
    long long number = 0;
    while( isdigit( input[point] ) ){
      number = number * 10 + input[point] - '0';
      ++point;
    }

    Token t = {2, 0, number, point};
    return t;
  }
  else if( input[point] == '(' ){
    Token t = {3, 0, 0, point+1};
    return t;
  }
  else if( input[point] == ')' ){
    Token t = {4, 0, 0, point+1};
    return t;
  }
}


Token f(long long *ascii, const string &input, int point){
  Token t = next(input, point);
  long long value = 1;
  while( t.type != 0 && t.type != 4 ){
    //printf("%d %c %lld %d\n", t.type, t.text, t.number, t.nextPoint);
    switch( t.type ){
      case 1:
        ascii[t.text-'a'] += value;
        value = 1;
        break;
      case 2:
        value = t.number;
        break;
      case 3:
        long long nAscii[26] = {0};
        t = f(nAscii, input, t.nextPoint);
        for( int i = 0 ; i < 26 ; ++i ){
          ascii[i] += nAscii[i] * value;
        }
        value = 1;
        break;
    }

    t = next(input, t.nextPoint);
  }
  return t;
}

int main(void){
    long long ascii[26] = {0};
    string input;
    cin >> input;

    f(ascii, input, 0);

    for( int i = 0 ; i < 26 ; ++i ){
      printf("%c %lld\n", 'a'+i, ascii[i]);
    }

    return 0;
}
