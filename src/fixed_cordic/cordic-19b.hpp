#ifndef _cordic_19b_
#define _cordic_19b_

#include "cordic_fixed.hpp"

//Cordic in 19 bit signed fixed point math
//Function is valid for arguments in range -pi/2 -- pi/2
//for values pi/2--pi: value = half_pi-(theta-half_pi) and similarly for values -pi---pi/2
//
// 1.0 = 131072
// 1/k = 0.6072529350088812561694
// pi = 3.1415926536897932384626

// Constants
#define cordic_1K_19b 0x000136E9
#define half_pi_19b   0x0003243F
#define MUL_19b 131072.000000

int cordic_ctab_19b[] = {
    0x00019220, // (0.78539818525314331054688)
    0x0000ED63, // (0.46364760398864746093750)
    0x00007D6E, // (0.24497866630554199218750)
    0x00003FAB, // (0.12435499578714370727539)
    0x00001FF5, // (0.06241881102323532104492)
    0x00000FFF, // (0.03123983368277549743652)
    0x00000800, // (0.01562372874468564987183)
    0x00000400, // (0.00781234120950102806091)
    0x00000200, // (0.00390623020939528942108)
    0x00000100, // (0.00195312255527824163437)
    0x00000080, // (0.00097656220896169543266)
    0x00000040, // (0.00048828122089616954327)
    0x00000020, // (0.00024414062500000000000)
    0x00000010, // (0.00012207031250000000000)
    0x00000008, // (0.00006103515625000000000)
    0x00000004, // (0.00003051757812500000000)
    0x00000002, // (0.00001525878906250000000)
    0x00000001, // (0.00000762939453125000000)
    0x00000000  // (0.00000381469726562500000)
};


void cordic_19b( int theta, int *s, int *c )
{
  bool invSign = false;
  if( theta < -half_pi_19b )
  {
    theta  += 2 * half_pi_19b;
    invSign = true;
  }
  if( theta > half_pi_19b )
  {
    theta  -= 2 * half_pi_19b;
    invSign = true;
  }

  int x = cordic_1K_19b;
  int y = 0;
  int z = theta;
  for (int k = 0; k < 19; ++k)
  {
    const int d  = z >= 0 ? 0 : -1;
    const int tx = x - (((y >> k) ^ d) - d);
    const int ty = y + (((x >> k) ^ d) - d);
    const int tz = z - ((cordic_ctab_19b[k] ^ d) - d);
    x  = tx;
    y  = ty;
    z  = tz;
  }
  (*c) = (invSign == true)? -x : x;
  (*s) = (invSign == true)? -y : y;
}

int float_to_fixed_19b(const float value)
{
  return value * MUL_19b;
}

float fixed_19b_to_float(const int value)
{
  return ((float)value) / MUL_19b;
}

class c_cordic_19b : public cordic_fixed
{
  void sincos(const float value, float* f_sinv, float* f_cosv)
  {
    int i_sin, i_cos;
    const int i_angle = float_to_fixed_19b( value );
    cordic_19b( i_angle, &i_sin, &i_cos );
    (*f_sinv) = fixed_19b_to_float( i_sin );
    (*f_cosv) = fixed_19b_to_float( i_cos );
  }

  int width(){ return 19; }

};

#endif

