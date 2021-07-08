#include <stdio.h>


int main(){
    //double-precision: double
    //single-precision: float
    //long-long
    
    //z = (x*x*x*x) - 4*(y*y*y*y) - 4*(y*y*)
    
    //1. double-precision:
    double x1 = 665857;
    double y1 = 470832;
    double z1 = (x1*x1*x1*x1) - 4*(y1*y1*y1*y1) - 4*(y1*y1);
    printf("Double precision: %lf\n",z1);
    
    //2. single-precision:
    float x2 = 665857;
    float y2 = 470832;
    float z2 = (x2*x2*x2*x2) - 4*(y2*y2*y2*y2) - 4*(y2*y2);
    
    printf("Simple precision: %f\n",z2);
    
    //3. long-long:
    long long x3 = 665857;
    long long y3 = 470832;
    long long z3 = (x3*x3*x3*x3) - 4*(y3*y3*y3*y3) - 4*(y3*y3);
    
    printf("Long long: %lld\n",z3);
}
