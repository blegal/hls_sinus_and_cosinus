#ifndef _cordic_10b_
#define _cordic_10b_

#include "cordic_fixed.hpp"

//Cordic in 10 bit signed fixed point math
//Function is valid for arguments in range -pi/2 -- pi/2
//for values pi/2--pi: value = half_pi-(theta-half_pi) and similarly for values -pi---pi/2
//
// 1.0 = 256
// 1/k = 0.6072529350088812561694
// pi = 3.1415926536897932384626

// Constants
#define cordic_1K_10b 0x0000009B
#define half_pi_10b   0x00000192
#define MUL_10b 256.000000

int cordic_ctab_10b[] = {
    0x000000C9, // (0.78539818525314331054688)
    0x00000077, // (0.46364760398864746093750)
    0x0000003F, // (0.24497866630554199218750)
    0x00000020, // (0.12435499578714370727539)
    0x00000010, // (0.06241881102323532104492)
    0x00000008, // (0.03123983368277549743652)
    0x00000004, // (0.01562372874468564987183)
    0x00000002, // (0.00781234120950102806091)
    0x00000001, // (0.00390623020939528942108)
    0x00000000  // (0.00195312255527824163437)
};


void cordic_10b( int theta, int *s, int *c )
{
  bool invSign = false;
  if( theta < -half_pi_10b )
  {
    theta  += 2 * half_pi_10b;
    invSign = true;
  }
  if( theta > half_pi_10b )
  {
    theta  -= 2 * half_pi_10b;
    invSign = true;
  }

  int x = cordic_1K_10b;
  int y = 0;
  int z = theta;
  for (int k = 0; k < 10; ++k)
  {
    const int d  = z >= 0 ? 0 : -1;
    const int tx = x - (((y >> k) ^ d) - d);
    const int ty = y + (((x >> k) ^ d) - d);
    const int tz = z - ((cordic_ctab_10b[k] ^ d) - d);
    x  = tx;
    y  = ty;
    z  = tz;
  }
  (*c) = (invSign == true)? -x : x;
  (*s) = (invSign == true)? -y : y;
}

int float_to_fixed_10b(const float value)
{
  return value * MUL_10b;
}

float fixed_10b_to_float(const int value)
{
  return ((float)value) / MUL_10b;
}

class c_cordic_10b : public cordic_fixed
{
  void sincos(const float value, float* f_sinv, float* f_cosv)
  {
    int i_sin, i_cos;
    const int i_angle = float_to_fixed_10b( value );
    cordic_10b( i_angle, &i_sin, &i_cos );
    (*f_sinv) = fixed_10b_to_float( i_sin );
    (*f_cosv) = fixed_10b_to_float( i_cos );
  }

  int width(){ return 10; }

};

#endif

