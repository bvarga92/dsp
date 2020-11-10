#include "fr1x16.h"

#define N 401  //szuroegyutthatok szama

extern fract16 inLeft, inRight, outLeft, outRight; //a be- es kimeno sztereo jelparok

fract16 input[N];
fract16 coeffs[N] = {0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0xFFFE, 0xFFFE, 0x0000, 0x0001, 0xFFFF, 0xFFFE, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0xFFFE, 0xFFFE, 0x0001, 0x0003, 0xFFFF, 0xFFFB, 0xFFFD, 0x0003, 0x0004, 0xFFFE, 0xFFFB, 0xFFFF, 0x0001, 0x0000, 0x0000, 0x0002, 0x0000, 0xFFF9, 0xFFFB, 0x0007, 0x000A, 0xFFFC, 0xFFF2, 0xFFFC, 0x000B, 0x0008, 0xFFFA, 0xFFF7, 0x0000, 0x0002, 0xFFFF, 0x0004, 0x0009, 0xFFFC, 0xFFEC, 0xFFFA, 0x0017, 0x0014, 0xFFF0, 0xFFE1, 0x0000, 0x001B, 0x000C, 0xFFF1, 0xFFF4, 0x0001, 0xFFFD, 0xFFFC, 0x0012, 0x0016, 0xFFEB, 0xFFD0, 0x0000, 0x003B, 0x001F, 0xFFD1, 0xFFC9, 0x000F, 0x0033, 0x000A, 0xFFE7, 0xFFF7, 0xFFFE, 0xFFEA, 0x0000, 0x003A, 0x0026, 0xFFBD, 0xFFA3, 0x001F, 0x007B, 0x001F, 0xFF97, 0xFFB1, 0x0031, 0x004A, 0xFFFF, 0xFFE7, 0x0003, 0xFFE3, 0xFFBF, 0x001C, 0x008D, 0x002B, 0xFF56, 0xFF6A, 0x0070, 0x00D9, 0xFFFE, 0xFF3B, 0xFFA8, 0x0065, 0x0055, 0xFFF2, 0x0008, 0x0011, 0xFF8E, 0xFF78, 0x007E, 0x0125, 0xFFFE, 0xFE8E, 0xFF38, 0x0122, 0x0158, 0xFF9E, 0xFEBA, 0xFFBC, 0x00A1, 0x0041, 0x0004, 0x0076, 0xFFFF, 0xFEA6, 0xFF15, 0x0199, 0x0242, 0xFF3E, 0xFCE7, 0xFF2E, 0x02BB, 0x021A, 0xFEA8, 0xFDDD, 0x0000, 0x00D2, 0xFFFA, 0x00C1, 0x01E7, 0xFF1B, 0xFB46, 0xFE6F, 0x066F, 0x0632, 0xFAF3, 0xF511, 0x0001, 0x0D03, 0x0729, 0xF55E, 0xF28B, 0x041A, 0x0FF4, 0x041A, 0xF28B, 0xF55E, 0x0729, 0x0D03, 0x0001, 0xF511, 0xFAF3, 0x0632, 0x066F, 0xFE6F, 0xFB46, 0xFF1B, 0x01E7, 0x00C1, 0xFFFA, 0x00D2, 0x0000, 0xFDDD, 0xFEA8, 0x021A, 0x02BB, 0xFF2E, 0xFCE7, 0xFF3E, 0x0242, 0x0199, 0xFF15, 0xFEA6, 0xFFFF, 0x0076, 0x0004, 0x0041, 0x00A1, 0xFFBC, 0xFEBA, 0xFF9E, 0x0158, 0x0122, 0xFF38, 0xFE8E, 0xFFFE, 0x0125, 0x007E, 0xFF78, 0xFF8E, 0x0011, 0x0008, 0xFFF2, 0x0055, 0x0065, 0xFFA8, 0xFF3B, 0xFFFE, 0x00D9, 0x0070, 0xFF6A, 0xFF56, 0x002B, 0x008D, 0x001C, 0xFFBF, 0xFFE3, 0x0003, 0xFFE7, 0xFFFF, 0x004A, 0x0031, 0xFFB1, 0xFF97, 0x001F, 0x007B, 0x001F, 0xFFA3, 0xFFBD, 0x0026, 0x003A, 0x0000, 0xFFEA, 0xFFFE, 0xFFF7, 0xFFE7, 0x000A, 0x0033, 0x000F, 0xFFC9, 0xFFD1, 0x001F, 0x003B, 0x0000, 0xFFD0, 0xFFEB, 0x0016, 0x0012, 0xFFFC, 0xFFFD, 0x0001, 0xFFF4, 0xFFF1, 0x000C, 0x001B, 0x0000, 0xFFE1, 0xFFF0, 0x0014, 0x0017, 0xFFFA, 0xFFEC, 0xFFFC, 0x0009, 0x0004, 0xFFFF, 0x0002, 0x0000, 0xFFF7, 0xFFFA, 0x0008, 0x000B, 0xFFFC, 0xFFF2, 0xFFFC, 0x000A, 0x0007, 0xFFFB, 0xFFF9, 0x0000, 0x0002, 0x0000, 0x0000, 0x0001, 0xFFFF, 0xFFFB, 0xFFFE, 0x0004, 0x0003, 0xFFFD, 0xFFFB, 0xFFFF, 0x0003, 0x0001, 0xFFFE, 0xFFFE, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFE, 0xFFFF, 0x0001, 0x0000, 0xFFFE, 0xFFFE, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF};

void init(){}

void process(){
    static int i = 0;
    int j, ix = i;
    fract16 out = 0;
    input[i] = inRight;	
    for(j=0; j<N; j++) out = add_fr1x16(out, mult_fr1x16(input[ix++%N], coeffs[j])); 
    i = (i==0) ? (N-1) : (i-1);
    outLeft = inRight; //a bal csatornan atengedjuk a bemeno jelet
    outRight = out; //a jobb csatornan lesz a szurt jel
}
