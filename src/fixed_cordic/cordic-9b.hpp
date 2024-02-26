#ifndef _cordic_9b_
#define _cordic_9b_

#include "cordic_fixed.hpp"

//Cordic in 9 bit signed fixed point math
//Function is valid for arguments in range -pi/2 -- pi/2
//for values pi/2--pi: value = half_pi-(theta-half_pi) and similarly for values -pi---pi/2
//
// 1.0 = 128
// 1/k = 0.6072529350088812561694
// pi = 3.1415926536897932384626

// Constants
#define cordic_1K_9b 0x0000004D
#define half_pi_9b   0x000000C9
#define MUL_9b 128.000000

int cordic_ctab_9b[] = {
    0x00000065, // (0.78539818525314331054688)
    0x0000003B, // (0.46364760398864746093750)
    0x0000001F, // (0.24497866630554199218750)
    0x00000010, // (0.12435499578714370727539)
    0x00000008, // (0.06241881102323532104492)
    0x00000004, // (0.03123983368277549743652)
    0x00000002, // (0.01562372874468564987183)
    0x00000001, // (0.00781234120950102806091)
    0x00000000  // (0.00390623020939528942108)
};


void cordic_9b( int theta, int *s, int *c )
{
  bool invSign = false;
  if( theta < -half_pi_9b )
  {
    theta  += 2 * half_pi_9b;
    invSign = true;
  }
  if( theta > half_pi_9b )
  {
    theta  -= 2 * half_pi_9b;
    invSign = true;
  }

  int x = cordic_1K_9b;
  int y = 0;
  int z = theta;
  for (int k = 0; k < 9; ++k)
  {
    const int d  = z >= 0 ? 0 : -1;
    const int tx = x - (((y >> k) ^ d) - d);
    const int ty = y + (((x >> k) ^ d) - d);
    const int tz = z - ((cordic_ctab_9b[k] ^ d) - d);
    x  = tx;
    y  = ty;
    z  = tz;
  }
  (*c) = (invSign == true)? -x : x;
  (*s) = (invSign == true)? -y : y;
}

int float_to_fixed_9b(const float value)
{
  return value * MUL_9b;
}

float fixed_9b_to_float(const int value)
{
  return ((float)value) / MUL_9b;
}

class c_cordic_9b : public cordic_fixed
{
  void sincos(const float value, float* f_sinv, float* f_cosv)
  {
    int i_sin, i_cos;
    const int i_angle = float_to_fixed_9b( value );
    cordic_9b( i_angle, &i_sin, &i_cos );
    (*f_sinv) = fixed_9b_to_float( i_sin );
    (*f_cosv) = fixed_9b_to_float( i_cos );
  }

  int width(){ return 9; }

};

#endif

