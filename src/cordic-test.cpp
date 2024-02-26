//#include "./fixed_cordic/cordic-32bit.h"
#include "./fixed_cordic/cordic-6b.hpp"
#include "./fixed_cordic/cordic-7b.hpp"
#include "./fixed_cordic/cordic-8b.hpp"
#include "./fixed_cordic/cordic-9b.hpp"
#include "./fixed_cordic/cordic-10b.hpp"
#include "./fixed_cordic/cordic-11b.hpp"
#include "./fixed_cordic/cordic-12b.hpp"
#include "./fixed_cordic/cordic-13b.hpp"
#include "./fixed_cordic/cordic-14b.hpp"
#include "./fixed_cordic/cordic-15b.hpp"
#include "./fixed_cordic/cordic-16b.hpp"
#include "./fixed_cordic/cordic-17b.hpp"
#include "./fixed_cordic/cordic-18b.hpp"
#include "./fixed_cordic/cordic-19b.hpp"
#include "./fixed_cordic/cordic-20b.hpp"
#include "./fixed_cordic/cordic-21b.hpp"
#include "./fixed_cordic/cordic-22b.hpp"
#include "./fixed_cordic/cordic-23b.hpp"
#include "./fixed_cordic/cordic-24b.hpp"
#include "./fixed_cordic/cordic-25b.hpp"
#include "./fixed_cordic/cordic-26b.hpp"
#include "./fixed_cordic/cordic-27b.hpp"
#include "./fixed_cordic/cordic-28b.hpp"
#include "./fixed_cordic/cordic-29b.hpp"
#include "./fixed_cordic/cordic-30b.hpp"
#include "./fixed_cordic/cordic-31b.hpp"

#include <cmath>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>


//Print out sin(x) vs fp CORDIC sin(x)
int main(int argc, char **argv)
{
    std::vector<cordic_fixed*> liste;

    liste.push_back( new c_cordic_8b () );
    liste.push_back( new c_cordic_9b () );
    liste.push_back( new c_cordic_10b() );
    liste.push_back( new c_cordic_11b() );
    liste.push_back( new c_cordic_12b() );
    liste.push_back( new c_cordic_13b() );
    liste.push_back( new c_cordic_14b() );
    liste.push_back( new c_cordic_15b() );
    liste.push_back( new c_cordic_16b() );
    liste.push_back( new c_cordic_17b() );
    liste.push_back( new c_cordic_18b() );
    liste.push_back( new c_cordic_19b() );
    liste.push_back( new c_cordic_20b() );
    liste.push_back( new c_cordic_21b() );
    liste.push_back( new c_cordic_22b() );
    liste.push_back( new c_cordic_23b() );
    liste.push_back( new c_cordic_24b() );
    liste.push_back( new c_cordic_25b() );
    liste.push_back( new c_cordic_26b() );
    liste.push_back( new c_cordic_27b() );
    liste.push_back( new c_cordic_28b() );
    liste.push_back( new c_cordic_29b() );
    liste.push_back( new c_cordic_30b() );
    liste.push_back( new c_cordic_31b() );

    const int nTaps = 64;

    for(int kk = 0; kk < liste.size(); kk += 1)
    {

        float t_radian[nTaps];
        float t_degree[nTaps];

        float t_ref_cos[nTaps];
        float t_ref_sin[nTaps];

        float t_cor_cos[nTaps];
        float t_cor_sin[nTaps];

        for(int i = 0; i < nTaps; i++)
        {
            const float   rad_f32 = ( (float)i / (float)nTaps) * (2 * M_PI) - M_PI;
            const float   deg_f32 = rad_f32 / M_PI * 180.f;
            //const int32_t rad_i32 = rad_f32 * MUL_28b;

            t_radian[ i ] = rad_f32;
            t_degree[ i ] = deg_f32;

            t_ref_sin[ i ] = sinf( rad_f32 );
            t_ref_cos[ i ] = cosf( rad_f32 );

            //
            // use 32 iterations
            //
            float s, c;
            liste[kk]->sincos( rad_f32, &s, &c );
            t_cor_sin[ i ] = s;
            t_cor_cos[ i ] = c;
        }

#if 0
        for(int i = 0; i < nTaps; i++)
        {
            printf("deg: %8.3f | rad: %8.3f | sin: %9.6f / %9.6f (%1.3e) | cos: %9.6f / %9.6f (%1.3e) |\n",
            t_degree[i], t_radian[i],
            t_ref_sin[i], t_cor_sin[i],
            abs(t_ref_sin[i] - t_cor_sin[i]),
            t_ref_cos[i], t_cor_cos[i],
            abs(t_ref_cos[i] - t_cor_cos[i]));
        }
#endif

        float err_min_c = +2.f;
        float err_min_s = +2.f;

        float err_max_c = -2.f;
        float err_max_s = -2.f;

        float err_avg_c =  0.f;
        float err_avg_s =  0.f;

        for(int i = 0; i < nTaps; i++)
        {
            const float err_sin = abs(t_ref_sin[i] - t_cor_sin[i]);
            err_min_s  = fmin( err_min_s, err_sin );
            err_max_s  = fmax( err_max_s, err_sin );
            err_avg_s += err_sin;

            const float err_cos = abs(t_ref_cos[i] - t_cor_cos[i]);
            err_min_c  = fmin( err_min_c, err_cos );
            err_max_c  = fmax( err_max_c, err_cos );
            err_avg_c += err_cos;
        }
        err_avg_s /= (float)nTaps;
        err_avg_c /= (float)nTaps;

        printf("STATS %2d : min: %1.3e | max: %1.3e | avg: %1.3e\n", liste[kk]->width(), err_min_c, err_max_c, err_avg_c );
        printf("           min: %1.3e | max: %1.3e | avg: %1.3e\n",    err_min_s, err_max_s, err_avg_s );;
    }

    //these values should be nearly equal

    return EXIT_SUCCESS;
}
