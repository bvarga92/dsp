#include "fr1x16.h"

#define L    96 //maximalis kesleltetes (egesz)
#define W     1 //a kesleltetes modulalasanak frekvenciaja (fract16)
#define G 16384 //keveresi arany (fract16)

extern fract16 inLeft, inRight, outLeft, outRight; //a be- es kimeno sztereo jelparok

fract16 buf[L + 1]; //cirkularis buffer

void init(){}

/* szinuszos flanger: https://ccrma.stanford.edu/~jos/pasp/Flanging.html */
void process(){
    static int idx_cur = 0; //az aktualis minta indexe
    static fract16 phi = 0; //a modulalojel fazisa
    int delay, idx_delay;
    buf[idx_cur] = add_fr1x16(mult_fr1x16(16384, inLeft), mult_fr1x16(16384, inRight));
    delay = (((L / 2) * (32767 - cos2pi_fr16(phi)))) >> 15;
    phi = (phi + W) & 0x7FFF;
    idx_delay = idx_cur - delay;
    if(idx_delay < 0)
        idx_delay += L + 1;
    outLeft = outRight = add_fr1x16(mult_fr1x16(G, buf[idx_delay]), mult_fr1x16(sub_fr1x16(32767, G), buf[idx_cur]));
    idx_cur = (idx_cur == L) ? 0 : (idx_cur + 1);
}
