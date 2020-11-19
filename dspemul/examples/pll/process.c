#include "fr1x16.h"

#define F_VCO 341 //VCO szabadonfuto frekvenciaja a mintaveteli frekvenciahoz viszonyitva (fract16)
#define K_VCO 137 //VCO erositese a mintaveteli frekvenciahoz viszonyitva (fract16)
#define N_LPF 101 //FIR szuro merete (egesz)

extern fract16 inLeft, inRight, outLeft, outRight; //a be- es kimeno sztereo jelparok

fract16 lpf_coeffs[N_LPF] = {46,46,48,51,54,59,65,72,80,89,98,109,120,133,146,160,174,189,205,221,238,255,272,290,307,325,343,361,379,396,413,430,447,463,479,494,508,521,534,546,557,568,577,585,593,599,604,608,611,612,613,612,611,608,604,599,593,585,577,568,557,546,534,521,508,494,479,463,447,430,413,396,379,361,343,325,307,290,272,255,238,221,205,189,174,160,146,133,120,109,98,89,80,72,65,59,54,51,48,46,46};
fract16 pd_buf[N_LPF];

void init(){}

void process(){
    static fract16 vco_out = 0, phi = 0, x = 0;
    fract16 pd_out, filter_out, dphi;
    static int i = 0;
    int j, ix, phi_ext;
    /* szorzos fazisdetektor */
    pd_out = mult_fr1x16(inLeft, vco_out);
    /* szuro */
    ix = i;
    filter_out = 0;
    pd_buf[i] = pd_out;
    for(j=0; j<N_LPF; j++) filter_out = add_fr1x16(filter_out, mult_fr1x16(pd_buf[ix++%N_LPF], lpf_coeffs[j])); 
    i = (i==0) ? (N_LPF-1) : (i-1);
    /* VCO hangolasa a hibajel alapjan */
    phi_ext = (int)phi + (int)mult_fr1x16(filter_out, K_VCO);
    if(phi_ext > 32767) phi_ext -= 32767;
    if(phi_ext < 0) phi_ext += 32767;
    phi = phi_ext;
    x = (x + F_VCO) & 0x7FFF;
    vco_out = sin2pi_fr16((x + phi) & 0x7FFF);
    /* bal csatornan a VCO kimenete, jobb csatornan a szuro kimenete (hibajel) */
    outLeft = vco_out;
    outRight = filter_out;
}
