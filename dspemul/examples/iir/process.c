#include "fr1x16.h"

#define B0 2412
#define B1 7235
#define B2 7235
#define B3 2412
#define A1 -31986
#define A2 28074
#define A3 -9564

extern fract16 inLeft, inRight, outLeft, outRight; //a be- es kimeno sztereo jelparok

void init(){}

void process(){
    static fract16 in1, in2, in3, out1, out2, out3;
    fract16 out = 0;
    out = add_fr1x16(out, mult_fr1x16(B0, inRight));
    out = add_fr1x16(out, mult_fr1x16(B1, in1));
    out = add_fr1x16(out, mult_fr1x16(B2, in2));
    out = add_fr1x16(out, mult_fr1x16(B3, in3));
    out = sub_fr1x16(out, mult_fr1x16(A1, out1));
    out = sub_fr1x16(out, mult_fr1x16(A2, out2));
    out = sub_fr1x16(out, mult_fr1x16(A3, out3));
    out3 = out2;
    out2 = out1;
    out1 = out;
    in3 = in2;
    in2 = in1;
    in1 = inRight;
    outLeft = inRight; //a bal csatornan atengedjuk a bemeno jelet
    outRight = out; //a jobb csatornan lesz a szurt jel
}
