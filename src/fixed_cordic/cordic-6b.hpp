#ifndef _cordic_6b_
#define _cordic_6b_

#include "cordic_fixed.hpp"

//Cordic in 6 bit signed fixed point math
//Function is valid for arguments in range -pi/2 -- pi/2
//for values pi/2--pi: value = half_pi-(theta-half_pi) and similarly for values -pi---pi/2
//
// 1.0 = 16
// 1/k = 0.6072529350088812561694
// pi = 3.1415926536897932384626

// Constants
#define cordic_1K_6b 0x00000009
#define half_pi_6b   0x00000019
#define MUL_6b 16.000000

int cordic_ctab_6b[] = {
    0x0000000D, // (0.78539818525314331054688)
    0x00000007, // (0.46364760398864746093750)
    0x00000004, // (0.24497866630554199218750)
    0x00000002, // (0.12435499578714370727539)
    0x00000001, // (0.06241881102323532104492)
    0x00000000  // (0.03123983368277549743652)
};


void cordic_6b( int theta, int *s, int *c )
{
  bool invSign = false;
  if( theta < -half_pi_6b )
  {
    theta  += 2 * half_pi_6b;
    invSign = true;
  }
  if( theta > half_pi_6b )
  {
    theta  -= 2 * half_pi_6b;
    invSign = true;
  }

  int x = cordic_1K_6b;
  int y = 0;
  int z = theta;
  for (int k = 0; k < 6; ++k)
  {
    const int d  = z >= 0 ? 0 : -1;
    const int tx = x - (((y >> k) ^ d) - d);
    const int ty = y + (((x >> k) ^ d) - d);
    const int tz = z - ((cordic_ctab_6b[k] ^ d) - d);
    x  = tx;
    y  = ty;
    z  = tz;
  }
  (*c) = (invSign == true)? -x : x;
  (*s) = (invSign == true)? -y : y;
}

int float_to_fixed_6b(const float value)
{
  return value * MUL_6b;
}

float fixed_6b_to_float(const int value)
{
  return ((float)value) / MUL_6b;
}

class c_cordic_6b : public cordic_fixed
{
  void sincos(const float value, float* f_sinv, float* f_cosv)
  {
    int i_sin, i_cos;
    const int i_angle = float_to_fixed_6b( value );
    cordic_6b( i_angle, &i_sin, &i_cos );
    (*f_sinv) = fixed_6b_to_float( i_sin );
    (*f_cosv) = fixed_6b_to_float( i_cos );
  }

  int width(){ return 6; }

};

#endif

