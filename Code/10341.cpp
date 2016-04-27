/*******************************************************/
/* UVa 10341 Solve It                                  */
/* Author: LanyiKnight [at] knightzone.org             */
/* Version: 2016/04/20                                 */
/*******************************************************/
#include <iostream>
#include <cstdio>
#include <cmath>
using namespace std;

bool isEqual(double a, double b){
  return (a - b) < 1e-9 && (a - b) > -1e-9;
}

struct Formula{
  double p;
  double q;
  double r;
  double s;
  double t;
  double u;

  double calculate(double x){
    return p * exp(-x) + q * sin(x) + r * cos(x) + s * tan(x) + t * x * x + u;
  }
};

int main(){
  Formula f;
  while( scanf("%lf%lf%lf%lf%lf%lf", &(f.p), &(f.q), &(f.r), &(f.s), &(f.t), &(f.u)) != EOF){
    double lowerBound = 0, upperBound = 1;
    double lowerValue = f.calculate(lowerBound), upperValue = f.calculate(upperBound);
    if( isEqual(lowerValue, 0) ){
      printf("%.4lf\n", lowerBound);
      continue;
    }

    if( isEqual(upperValue, 0) ){
      printf("%.4lf\n", upperBound);
      continue;
    }

    if( lowerValue * upperValue > 0 ){
      printf("No solution\n");
      continue;
    }

    double mid;
    double midValue;
    do{
      mid = (lowerBound + upperBound) / 2;
      midValue = f.calculate(mid);

      if( midValue < 0 ){
        upperBound = mid;
      }
      else {
        lowerBound = mid;
      }
    } while( !isEqual(midValue, 0) );

    printf("%.4lf\n", mid);

  }

  return 0;
}
