#include "cordic-16bit.h"
#include <cmath>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

//Print out sin(x) vs fp CORDIC sin(x)
int main(int argc, char **argv)
{
    const int taps = 64;
    double p;
    int s,c;
    int i;    
    for(int i = 0; i < taps; i++)
    {
        p = (i/64.0)*M_PI/2;        
        //use 32 iterations
        cordic((p*MUL), &s, &c, 32);
        //these values should be nearly equal
        printf("%9.9f : %9.9f => err = %9.9f\n", (s/MUL), sin(p), (s/MUL) - sin(p));
    }
}
