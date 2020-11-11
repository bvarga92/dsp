#include "fr1x16.h"

#define L 183

extern fract16 inLeft, inRight, outLeft, outRight; //a be- es kimeno sztereo jelparok

fract16 buf[L] = {18409,-7789,21538,18460,19765,-9378,30286,-9421,20878,26554,-29965,-31707,3673,-27491,30426,-5996,-11434,-2331,24347,-9634,-18631,-19551,-16971,-18715,-16306,-11745,2413,-27474,-29989,13533,29795,7436,12253,-18469,24702,-22327,724,-19075,-9179,-27030,-24814,32433,923,12009,-25524,8914,-8194,-31704,9407,-26141,-22331,12939,-14244,21823,28506,-19886,-4394,21381,-19771,20747,-27396,-6437,31038,24912,-30195,-12198,-32186,-28324,-11114,-29625,31432,4830,16534,25201,20624,-14464,26666,-11473,28342,-3344,4754,-19342,24022,-14937,1327,-1722,739,-12288,22949,24725,19994,6373,-26565,-6443,-27595,9347,-10014,-23422,-12765,15968,12594,-27066,21478,495,2292,-11469,-27919,-1139,-13881,16380,18369,1548,24957,-5933,7123,-5313,25652,-8630,-22522,-6832,-28599,20850,4585,-27021,6025,17479,14565,-26807,-28698,-2556,18097,23530,-18895,27998,-8887,15933,22923,-29612,-13881,11658,25167,-15213,-32340,27702,18009,10072,6287,7664,-26421,32453,10590,-31392,10059,-2126,-17508,-5688,-24319,28018,-18392,29059,25116,-466,-6457,11407,11396,-9195,-25792,28132,11478,-16665,22690,28035,-4026,21000,-14286,4781,-196,-3210,15556,-12810,-23151,30253,16024};

void init(){}

/* Megpengetett hur hangjanak szintezise Karplus-Strong-algoritmussal: https://ccrma.stanford.edu/realsimple/faust_strings/Karplus_Strong_Digitar_Algorithm.html
   Nincs input.dat, futtatas: main.exe 192000
*/
void process(){
    static int idx = 0;
    static fract16 dl = 0;
    outLeft = outRight = mult_fr1x16(16384, add_fr1x16(buf[idx], dl));
    dl = buf[idx];
    buf[idx] = outRight;
    idx = (idx + 1) % L;
}
