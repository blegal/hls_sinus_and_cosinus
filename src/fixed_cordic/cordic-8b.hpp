#ifndef _cordic_8b_
#define _cordic_8b_

#include "cordic_fixed.hpp"

//Cordic in 8 bit signed fixed point math
//Function is valid for arguments in range -pi/2 -- pi/2
//for values pi/2--pi: value = half_pi-(theta-half_pi) and similarly for values -pi---pi/2
//
// 1.0 = 64
// 1/k = 0.6072529350088812561694
// pi = 3.1415926536897932384626

// Constants
#define cordic_1K_8b 0x00000026
#define half_pi_8b   0x00000064
#define MUL_8b 64.000000

int cordic_ctab_8b[] = {
    0x00000032, // (0.78539818525314331054688)
    0x0000001E, // (0.46364760398864746093750)
    0x00000010, // (0.24497866630554199218750)
    0x00000008, // (0.12435499578714370727539)
    0x00000004, // (0.06241881102323532104492)
    0x00000002, // (0.03123983368277549743652)
    0x00000001, // (0.01562372874468564987183)
    0x00000000  // (0.00781234120950102806091)
};


void cordic_8b( int theta, int *s, int *c )
{
  bool invSign = false;
  if( theta < -half_pi_8b )
  {
    theta  += 2 * half_pi_8b;
    invSign = true;
  }
  if( theta > half_pi_8b )
  {
    theta  -= 2 * half_pi_8b;
    invSign = true;
  }

  int x = cordic_1K_8b;
  int y = 0;
  int z = theta;
  for (int k = 0; k < 8; ++k)
  {
    const int d  = z >= 0 ? 0 : -1;
    const int tx = x - (((y >> k) ^ d) - d);
    const int ty = y + (((x >> k) ^ d) - d);
    const int tz = z - ((cordic_ctab_8b[k] ^ d) - d);
    x  = tx;
    y  = ty;
    z  = tz;
  }
  (*c) = (invSign == true)? -x : x;
  (*s) = (invSign == true)? -y : y;
}

int float_to_fixed_8b(const float value)
{
  return value * MUL_8b;
}

float fixed_8b_to_float(const int value)
{
  return ((float)value) / MUL_8b;
}

class c_cordic_8b : public cordic_fixed
{
  void sincos(const float value, float* f_sinv, float* f_cosv)
  {
    int i_sin, i_cos;
    const int i_angle = float_to_fixed_8b( value );
    cordic_8b( i_angle, &i_sin, &i_cos );
    (*f_sinv) = fixed_8b_to_float( i_sin );
    (*f_cosv) = fixed_8b_to_float( i_cos );
  }

  int width(){ return 8; }

};

#endif

