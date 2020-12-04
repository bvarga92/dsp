#include "fr1x16.h"
#include <math.h>
#include <stdlib.h>

typedef struct{double re, im;} complex;

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

void twidfftrad2_fr16(complex_fract16 twiddle_table[], int fft_size){
    unsigned i;
    for(i=0; i<fft_size/2; i++){
        twiddle_table[i].re = float_to_fr16(cos(2 * M_PI / ((double)fft_size) * i));
        twiddle_table[i].im = float_to_fr16(-sin(2 * M_PI / ((double)fft_size) * i));
    }
}

void rfft_fr16(const fract16 *input, complex_fract16 *output, const complex_fract16 *twiddle_table, int fft_size){
    unsigned bits, i, irev, j, i1, i2, itw;
    complex temp1, temp2, *output_d;
    output_d = (complex*)malloc(fft_size * sizeof(complex));
    bits = (unsigned)log2(fft_size);
    /* bitreverse cimzes */
    for(i=0; i<fft_size; i++){
        irev = i;
        for(j=0; j<bits-1; j++) irev = ((irev<<1) & (fft_size-1)) | ((i>>(j+1)) & 1);
        output_d[irev].re = fr16_to_float(input[i]);
        output_d[irev].im = 0;
    }
    /* nonrekurziv FFT */
    for(i=0; i<bits; i++){ //hanyadik FFT szint
        for(j=0; j<fft_size/2; j++){ //szinten belul hanyadik butterfly muvelet
            i1 = ((j<<(i+1)) | (j>>(bits-i-1))) & (fft_size-1); //egyik bemenet indexe
            i2 = ((j<<(i+1)) | (1<<i) | (j>>(bits-i-1))) & (fft_size-1); //masik bemenet indexe
            itw = (j>>(bits-i-1))<<(bits-i-1); //egyutthato indexe
            temp1.re = output_d[i1].re + fr16_to_float(twiddle_table[itw].re)*output_d[i2].re - fr16_to_float(twiddle_table[itw].im)*output_d[i2].im;
            temp1.im = output_d[i1].im + fr16_to_float(twiddle_table[itw].re)*output_d[i2].im + fr16_to_float(twiddle_table[itw].im)*output_d[i2].re;
            temp2.re = output_d[i1].re - fr16_to_float(twiddle_table[itw].re)*output_d[i2].re + fr16_to_float(twiddle_table[itw].im)*output_d[i2].im;
            temp2.im = output_d[i1].im - fr16_to_float(twiddle_table[itw].re)*output_d[i2].im - fr16_to_float(twiddle_table[itw].im)*output_d[i2].re;
            output_d[i1] = temp1;
            output_d[i2] = temp2;
        }
    }
    /* skalazas es fract16 konverzio */
    for(i=0; i<fft_size; i++){
        output[i].re = float_to_fr16(output_d[i].re / fft_size);
        output[i].im = float_to_fr16(output_d[i].im / fft_size);
    }
    free(output_d);
}

fract16 cabs_fr16(complex_fract16 a){
    return float_to_fr16(hypot(fr16_to_float(a.re), fr16_to_float(a.im)));
}

void gen_blackman_fr16(fract16 blackman_window[], int window_size){
    unsigned i;
    for(i=0; i<window_size; i++)
        blackman_window[i] = float_to_fr16(0.42 - 0.5 * cos(2 * M_PI * i / ((double)(window_size-1))) + 0.08 * cos(4 * M_PI * i / ((double)(window_size-1))));
}
