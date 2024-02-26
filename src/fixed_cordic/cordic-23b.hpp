#ifndef _cordic_23b_
#define _cordic_23b_

#include "cordic_fixed.hpp"

//Cordic in 23 bit signed fixed point math
//Function is valid for arguments in range -pi/2 -- pi/2
//for values pi/2--pi: value = half_pi-(theta-half_pi) and similarly for values -pi---pi/2
//
// 1.0 = 2097152
// 1/k = 0.6072529350088812561694
// pi = 3.1415926536897932384626

// Constants
#define cordic_1K_23b 0x00136E9D
#define half_pi_23b   0x003243F6
#define MUL_23b 2097152.000000

int cordic_ctab_23b[] = {
    0x001921FB, // (0.78539818525314331054688)
    0x000ED634, // (0.46364760398864746093750)
    0x0007D6DD, // (0.24497866630554199218750)
    0x0003FAB7, // (0.12435499578714370727539)
    0x0001FF56, // (0.06241881102323532104492)
    0x0000FFEB, // (0.03123983368277549743652)
    0x00007FFD, // (0.01562372874468564987183)
    0x00004000, // (0.00781234120950102806091)
    0x00002000, // (0.00390623020939528942108)
    0x00001000, // (0.00195312255527824163437)
    0x00000800, // (0.00097656220896169543266)
    0x00000400, // (0.00048828122089616954327)
    0x00000200, // (0.00024414062500000000000)
    0x00000100, // (0.00012207031250000000000)
    0x00000080, // (0.00006103515625000000000)
    0x00000040, // (0.00003051757812500000000)
    0x00000020, // (0.00001525878906250000000)
    0x00000010, // (0.00000762939453125000000)
    0x00000008, // (0.00000381469726562500000)
    0x00000004, // (0.00000190734863281250000)
    0x00000002, // (0.00000095367431640625000)
    0x00000001, // (0.00000047683715820312500)
    0x00000000  // (0.00000023841857910156250)
};


void cordic_23b( int theta, int *s, int *c )
{
  bool invSign = false;
  if( theta < -half_pi_23b )
  {
    theta  += 2 * half_pi_23b;
    invSign = true;
  }
  if( theta > half_pi_23b )
  {
    theta  -= 2 * half_pi_23b;
    invSign = true;
  }

  int x = cordic_1K_23b;
  int y = 0;
  int z = theta;
  for (int k = 0; k < 23; ++k)
  {
    const int d  = z >= 0 ? 0 : -1;
    const int tx = x - (((y >> k) ^ d) - d);
    const int ty = y + (((x >> k) ^ d) - d);
    const int tz = z - ((cordic_ctab_23b[k] ^ d) - d);
    x  = tx;
    y  = ty;
    z  = tz;
  }
  (*c) = (invSign == true)? -x : x;
  (*s) = (invSign == true)? -y : y;
}

int float_to_fixed_23b(const float value)
{
  return value * MUL_23b;
}

float fixed_23b_to_float(const int value)
{
  return ((float)value) / MUL_23b;
}

class c_cordic_23b : public cordic_fixed
{
  void sincos(const float value, float* f_sinv, float* f_cosv)
  {
    int i_sin, i_cos;
    const int i_angle = float_to_fixed_23b( value );
    cordic_23b( i_angle, &i_sin, &i_cos );
    (*f_sinv) = fixed_23b_to_float( i_sin );
    (*f_cosv) = fixed_23b_to_float( i_cos );
  }

  int width(){ return 23; }

};

#endif

