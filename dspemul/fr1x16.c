#include "fr1x16.h"
#include <math.h>

double fr16_to_float(fract16 input){
    return input / 32768.0;
}

fract16 float_to_fr16(double input){
    double temp = round(input * 32768);
    if(temp > 32767)
        temp = 32767;
    else if(temp < -32768)
        temp = -32768;
    return (fract16)temp;
}

fract16 add_fr1x16(fract16 f1, fract16 f2){
    if((int)f1 + (int)f2 > 32767)
        return 32767;
    if((int)f1 + (int)f2 < -32768)
        return -32768;
    return f1 + f2;
}

fract16 sub_fr1x16(fract16 f1, fract16 f2){
    if(f2 == -32768)
        f2 = 32767;    
    else
        f2 = -f2;
    return add_fr1x16(f1, f2);
}

fract16 mult_fr1x16(fract16 f1, fract16 f2){
    if((f1 == -32768) && (f2 == -32768)) return 32767;
    return ((int)f1 * (int)f2) >> 15;
}

fract16 abs_fr1x16(fract16 f1){
    if(f1 == -32768) return 32767;
    if(f1 < 0) return -f1;
    return f1;
}

fract16 sin2pi_fr16(fract16 x){
    if(x < 0) return 0;
    return float_to_fr16(sin(2*M_PI*fr16_to_float(x)));    
}

fract16 cos2pi_fr16(fract16 x){
    if(x < 0) return 0;
    return float_to_fr16(cos(2*M_PI*fr16_to_float(x)));    
}
