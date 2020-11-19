#ifndef _FR1X16_H_
#define _FR1X16_H_

/* tipusok */
typedef short fract16;
typedef struct{fract16 re, im;} complex_fract16;

/* konverzio */
double fr16_to_float(fract16 input);
fract16 float_to_fr16(double input);

/* alapmuveletek */
fract16 add_fr1x16(fract16 f1, fract16 f2);
fract16 sub_fr1x16(fract16 f1, fract16 f2);
fract16 mult_fr1x16(fract16 f1, fract16 f2);
fract16 abs_fr1x16(fract16 f1);

/* sin(2pi*x) es cos(2pi*x), x nem lehet negativ! */
fract16 sin2pi_fr16(fract16 x);
fract16 cos2pi_fr16(fract16 x);

/* fuggvenyek FFT-hez, a meret csak kettohatvany lehet! */
void twidfftrad2_fr16(complex_fract16 twiddle_table[], int fft_size);
void rfft_fr16(const fract16 *input, complex_fract16 *output, const complex_fract16 *twiddle_table, int fft_size); //twiddle_stride, block_exponent, scale_method parameterek nincsenek implementalva (ertekeik: 1, NULL, 1)
fract16 cabs_fr16(complex_fract16 a);
void gen_blackman_fr16(fract16 blackman_window[], int window_size); //window_stride parameter nincs implementalva (erteke: 1)

#endif
