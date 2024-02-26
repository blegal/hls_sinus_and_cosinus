#include <cmath>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <cstdlib>


//#define M_PI 3.1415926536897932384626
#define K1 0.6072529350088812561694

int main(int argc, char **argv)
{
    int i;
    int bits       = 16; // number of bits 
    
    if( argc == 2 )
        bits = std::atoi( argv[1] );
    
    int mul = (1 << (bits-2));
    int n   = bits; // number of elements.
    int c;

    printf("#ifndef _cordic_%db_\n", bits);
    printf("#define _cordic_%db_\n", bits);
    printf("\n");
    printf("#include \"cordic_fixed.hpp\"\n");
    printf("\n");
    printf("//Cordic in %d bit signed fixed point math\n", bits);
    printf("//Function is valid for arguments in range -pi/2 -- pi/2\n");
    printf("//for values pi/2--pi: value = half_pi-(theta-half_pi) and similarly for values -pi---pi/2\n");
    printf("//\n");
    printf("// 1.0 = %d\n", mul);
    printf("// 1/k = 0.6072529350088812561694\n");
    printf("// pi = 3.1415926536897932384626\n");
    printf("\n");
    printf("// Constants\n");
    printf("#define cordic_1K_%db 0x%08X\n", bits, (int)(mul*K1));
    printf("#define half_pi_%db   0x%08X\n", bits, (int)(mul*(M_PI/2)));
    printf("#define MUL_%db %f\n",           bits, (double)mul);
    printf("\n");
    printf("int cordic_ctab_%db[] = {\n", bits);
    for( i = 0; i < n; i+= 1)
    {
        c = ( atan( pow(2, -i) ) * mul);
        if( i != (n-1) )
            printf("    0x%08X, // (%1.23f)\n", c, (float)atan( pow(2, -i) ));
        else
            printf("    0x%08X  // (%1.23f)\n", c, (float)atan( pow(2, -i) ));
    }
    printf("};\n");
    printf("\n");
    printf("\n");
    printf("void cordic_%db( int theta, int *s, int *c )\n{\n", bits);
    printf("  bool invSign = false;\n");
    printf("  if( theta < -half_pi_%db )\n", bits);
    printf("  {\n");
    printf("    theta  += 2 * half_pi_%db;\n", bits);
    printf("    invSign = true;\n");
    printf("  }\n");
    printf("  if( theta > half_pi_%db )\n", bits);
    printf("  {\n");
    printf("    theta  -= 2 * half_pi_%db;\n", bits);
    printf("    invSign = true;\n");
    printf("  }\n");
    printf("\n");
    printf("  int x = cordic_1K_%db;\n", bits);
    printf("  int y = 0;\n");
    printf("  int z = theta;\n");
    printf("  for (int k = 0; k < %d; ++k)\n", bits);
    printf("  {\n");
    printf("    const int d  = z >= 0 ? 0 : -1;\n");
    printf("    const int tx = x - (((y >> k) ^ d) - d);\n");
    printf("    const int ty = y + (((x >> k) ^ d) - d);\n");
    printf("    const int tz = z - ((cordic_ctab_%db[k] ^ d) - d);\n", bits);
    printf("    x  = tx;\n");
    printf("    y  = ty;\n");
    printf("    z  = tz;\n");
    printf("  }\n");
    printf("  (*c) = (invSign == true)? -x : x;\n");
    printf("  (*s) = (invSign == true)? -y : y;\n");
    printf("}\n");
    printf("\n");
    printf("int float_to_fixed_%db(const float value)\n", bits);
    printf("{\n");
    printf("  return value * MUL_%db;\n", bits);
    printf("}\n");
    printf("\n");
    printf("float fixed_%db_to_float(const int value)\n", bits);
    printf("{\n");
    printf("  return ((float)value) / MUL_%db;\n", bits);
    printf("}\n");
    printf("\n");
    printf("class c_cordic_%db : public cordic_fixed\n", bits);
    printf("{\n");
    printf("  void sincos(const float value, float* f_sinv, float* f_cosv)\n");
    printf("  {\n");
    printf("    int i_sin, i_cos;\n");
    printf("    const int i_angle = float_to_fixed_%db( value );\n", bits);
    printf("    cordic_%db( i_angle, &i_sin, &i_cos );\n", bits);
    printf("    (*f_sinv) = fixed_%db_to_float( i_sin );\n", bits);
    printf("    (*f_cosv) = fixed_%db_to_float( i_cos );\n", bits);
    printf("  }\n");
    printf("\n");
    printf("  int width(){ return %d; }\n", bits);
    printf("\n");
    printf("};\n");
    printf("\n");
    printf("#endif\n");
    printf("\n");
}
