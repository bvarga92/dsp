#include "fr1x16.h"

#define L  9600  //kesleltetovonal hossza (egesz)
#define FF 0     //elorecsatolas (fract16)
#define FB 6554  //visszacsatolas (fract16)
#define BL 32767 //blend (fract16)

extern fract16 inLeft, inRight, outLeft, outRight; //a be- es kimeno sztereo jelparok

fract16 dl[L]; //kesleltetovonal

void init(){}

/* univerzalis fesuszuro kulonbozo effektekhez (pl. echo, slapback) */
void process(){
    static unsigned idx = L - 1; //mindig a legregebbi mintara mutat
    fract16 in, out, temp;
    in = add_fr1x16(mult_fr1x16(16384, inLeft), mult_fr1x16(16384, inRight));
    temp = dl[idx];
    dl[idx] = add_fr1x16(in, mult_fr1x16(FB, temp));
    out = add_fr1x16(mult_fr1x16(FF, temp), mult_fr1x16(BL, dl[idx]));
    idx = idx ? (idx - 1) : (L - 1);
    outLeft = outRight = out;
}
