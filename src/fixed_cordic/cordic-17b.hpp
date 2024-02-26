#ifndef _cordic_17b_
#define _cordic_17b_

#include "cordic_fixed.hpp"

//Cordic in 17 bit signed fixed point math
//Function is valid for arguments in range -pi/2 -- pi/2
//for values pi/2--pi: value = half_pi-(theta-half_pi) and similarly for values -pi---pi/2
//
// 1.0 = 32768
// 1/k = 0.6072529350088812561694
// pi = 3.1415926536897932384626

// Constants
#define cordic_1K_17b 0x00004DBA
#define half_pi_17b   0x0000C90F
#define MUL_17b 32768.000000

int cordic_ctab_17b[] = {
    0x00006488, // (0.78539818525314331054688)
    0x00003B59, // (0.46364760398864746093750)
    0x00001F5B, // (0.24497866630554199218750)
    0x00000FEB, // (0.12435499578714370727539)
    0x000007FD, // (0.06241881102323532104492)
    0x00000400, // (0.03123983368277549743652)
    0x00000200, // (0.01562372874468564987183)
    0x00000100, // (0.00781234120950102806091)
    0x00000080, // (0.00390623020939528942108)
    0x00000040, // (0.00195312255527824163437)
    0x00000020, // (0.00097656220896169543266)
    0x00000010, // (0.00048828122089616954327)
    0x00000008, // (0.00024414062500000000000)
    0x00000004, // (0.00012207031250000000000)
    0x00000002, // (0.00006103515625000000000)
    0x00000001, // (0.00003051757812500000000)
    0x00000000  // (0.00001525878906250000000)
};


void cordic_17b( int theta, int *s, int *c )
{
  bool invSign = false;
  if( theta < -half_pi_17b )
  {
    theta  += 2 * half_pi_17b;
    invSign = true;
  }
  if( theta > half_pi_17b )
  {
    theta  -= 2 * half_pi_17b;
    invSign = true;
  }

  int x = cordic_1K_17b;
  int y = 0;
  int z = theta;
  for (int k = 0; k < 17; ++k)
  {
    const int d  = z >= 0 ? 0 : -1;
    const int tx = x - (((y >> k) ^ d) - d);
    const int ty = y + (((x >> k) ^ d) - d);
    const int tz = z - ((cordic_ctab_17b[k] ^ d) - d);
    x  = tx;
    y  = ty;
    z  = tz;
  }
  (*c) = (invSign == true)? -x : x;
  (*s) = (invSign == true)? -y : y;
}

int float_to_fixed_17b(const float value)
{
  return value * MUL_17b;
}

float fixed_17b_to_float(const int value)
{
  return ((float)value) / MUL_17b;
}

class c_cordic_17b : public cordic_fixed
{
  void sincos(const float value, float* f_sinv, float* f_cosv)
  {
    int i_sin, i_cos;
    const int i_angle = float_to_fixed_17b( value );
    cordic_17b( i_angle, &i_sin, &i_cos );
    (*f_sinv) = fixed_17b_to_float( i_sin );
    (*f_cosv) = fixed_17b_to_float( i_cos );
  }

  int width(){ return 17; }

};

#endif

