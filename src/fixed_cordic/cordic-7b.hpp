#ifndef _cordic_7b_
#define _cordic_7b_

#include "cordic_fixed.hpp"

//Cordic in 7 bit signed fixed point math
//Function is valid for arguments in range -pi/2 -- pi/2
//for values pi/2--pi: value = half_pi-(theta-half_pi) and similarly for values -pi---pi/2
//
// 1.0 = 32
// 1/k = 0.6072529350088812561694
// pi = 3.1415926536897932384626

// Constants
#define cordic_1K_7b 0x00000013
#define half_pi_7b   0x00000032
#define MUL_7b 32.000000

int cordic_ctab_7b[] = {
    0x00000019, // (0.78539818525314331054688)
    0x0000000F, // (0.46364760398864746093750)
    0x00000008, // (0.24497866630554199218750)
    0x00000004, // (0.12435499578714370727539)
    0x00000002, // (0.06241881102323532104492)
    0x00000001, // (0.03123983368277549743652)
    0x00000000  // (0.01562372874468564987183)
};


void cordic_7b( int theta, int *s, int *c )
{
  bool invSign = false;
  if( theta < -half_pi_7b )
  {
    theta  += 2 * half_pi_7b;
    invSign = true;
  }
  if( theta > half_pi_7b )
  {
    theta  -= 2 * half_pi_7b;
    invSign = true;
  }

  int x = cordic_1K_7b;
  int y = 0;
  int z = theta;
  for (int k = 0; k < 7; ++k)
  {
    const int d  = z >= 0 ? 0 : -1;
    const int tx = x - (((y >> k) ^ d) - d);
    const int ty = y + (((x >> k) ^ d) - d);
    const int tz = z - ((cordic_ctab_7b[k] ^ d) - d);
    x  = tx;
    y  = ty;
    z  = tz;
  }
  (*c) = (invSign == true)? -x : x;
  (*s) = (invSign == true)? -y : y;
}

int float_to_fixed_7b(const float value)
{
  return value * MUL_7b;
}

float fixed_7b_to_float(const int value)
{
  return ((float)value) / MUL_7b;
}

class c_cordic_7b : public cordic_fixed
{
  void sincos(const float value, float* f_sinv, float* f_cosv)
  {
    int i_sin, i_cos;
    const int i_angle = float_to_fixed_7b( value );
    cordic_7b( i_angle, &i_sin, &i_cos );
    (*f_sinv) = fixed_7b_to_float( i_sin );
    (*f_cosv) = fixed_7b_to_float( i_cos );
  }

  int width(){ return 7; }

};

#endif

