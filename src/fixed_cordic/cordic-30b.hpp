#ifndef _cordic_30b_
#define _cordic_30b_

#include "cordic_fixed.hpp"

//Cordic in 30 bit signed fixed point math
//Function is valid for arguments in range -pi/2 -- pi/2
//for values pi/2--pi: value = half_pi-(theta-half_pi) and similarly for values -pi---pi/2
//
// 1.0 = 268435456
// 1/k = 0.6072529350088812561694
// pi = 3.1415926536897932384626

// Constants
#define cordic_1K_30b 0x09B74EDA
#define half_pi_30b   0x1921FB54
#define MUL_30b 268435456.000000

int cordic_ctab_30b[] = {
    0x0C90FDAA, // (0.78539818525314331054688)
    0x076B19C1, // (0.46364760398864746093750)
    0x03EB6EBF, // (0.24497866630554199218750)
    0x01FD5BAA, // (0.12435499578714370727539)
    0x00FFAADE, // (0.06241881102323532104492)
    0x007FF557, // (0.03123983368277549743652)
    0x003FFEAB, // (0.01562372874468564987183)
    0x001FFFD5, // (0.00781234120950102806091)
    0x000FFFFB, // (0.00390623020939528942108)
    0x0007FFFF, // (0.00195312255527824163437)
    0x00040000, // (0.00097656220896169543266)
    0x00020000, // (0.00048828122089616954327)
    0x00010000, // (0.00024414062500000000000)
    0x00008000, // (0.00012207031250000000000)
    0x00004000, // (0.00006103515625000000000)
    0x00002000, // (0.00003051757812500000000)
    0x00001000, // (0.00001525878906250000000)
    0x00000800, // (0.00000762939453125000000)
    0x00000400, // (0.00000381469726562500000)
    0x00000200, // (0.00000190734863281250000)
    0x00000100, // (0.00000095367431640625000)
    0x00000080, // (0.00000047683715820312500)
    0x00000040, // (0.00000023841857910156250)
    0x00000020, // (0.00000011920928955078125)
    0x00000010, // (0.00000005960464477539062)
    0x00000008, // (0.00000002980232238769531)
    0x00000004, // (0.00000001490116119384766)
    0x00000002, // (0.00000000745058059692383)
    0x00000001, // (0.00000000372529029846191)
    0x00000001  // (0.00000000186264514923096)
};


void cordic_30b( int theta, int *s, int *c )
{
  bool invSign = false;
  if( theta < -half_pi_30b )
  {
    theta  += 2 * half_pi_30b;
    invSign = true;
  }
  if( theta > half_pi_30b )
  {
    theta  -= 2 * half_pi_30b;
    invSign = true;
  }

  int x = cordic_1K_30b;
  int y = 0;
  int z = theta;
  for (int k = 0; k < 30; ++k)
  {
    const int d  = z >= 0 ? 0 : -1;
    const int tx = x - (((y >> k) ^ d) - d);
    const int ty = y + (((x >> k) ^ d) - d);
    const int tz = z - ((cordic_ctab_30b[k] ^ d) - d);
    x  = tx;
    y  = ty;
    z  = tz;
  }
  (*c) = (invSign == true)? -x : x;
  (*s) = (invSign == true)? -y : y;
}

int float_to_fixed_30b(const float value)
{
  return value * MUL_30b;
}

float fixed_30b_to_float(const int value)
{
  return ((float)value) / MUL_30b;
}

class c_cordic_30b : public cordic_fixed
{
  void sincos(const float value, float* f_sinv, float* f_cosv)
  {
    int i_sin, i_cos;
    const int i_angle = float_to_fixed_30b( value );
    cordic_30b( i_angle, &i_sin, &i_cos );
    (*f_sinv) = fixed_30b_to_float( i_sin );
    (*f_cosv) = fixed_30b_to_float( i_cos );
  }

  int width(){ return 30; }

};

#endif

