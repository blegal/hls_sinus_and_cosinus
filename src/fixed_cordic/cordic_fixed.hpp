#ifndef _cordic_fixed_
#define _cordic_fixed_

class cordic_fixed
{
public:
  virtual void sincos(const float value, float* f_sinv, float* f_cosv) = 0;
  virtual  int width() = 0;
};

#endif