#ifndef _FR1X16_H_
#define _FR1X16_H_

typedef short fract16;

/* konverzio */
double fr16_to_float(fract16 input);
fract16 float_to_fr16(double input);

/* alapmuveletek */
fract16 add_fr1x16(fract16 f1, fract16 f2);
fract16 sub_fr1x16(fract16 f1, fract16 f2);
fract16 mult_fr1x16(fract16 f1, fract16 f2);
fract16 abs_fr1x16(fract16 f1);

/* sin(2pi*x), a bemenet nem lehet negativ! */
fract16 sin2pi_fr16(fract16 x);

#endif
