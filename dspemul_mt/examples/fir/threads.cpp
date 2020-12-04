#include "globals.hpp"

#define N 401  //szuroegyutthatok szama

fract16 input[N];
const fract16 coeffs[N] = {(fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFE, (fract16)0xFFFE, (fract16)0x0000, (fract16)0x0001, (fract16)0xFFFF, (fract16)0xFFFE, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFE, (fract16)0xFFFE, (fract16)0x0001, (fract16)0x0003, (fract16)0xFFFF, (fract16)0xFFFB, (fract16)0xFFFD, (fract16)0x0003, (fract16)0x0004, (fract16)0xFFFE, (fract16)0xFFFB, (fract16)0xFFFF, (fract16)0x0001, (fract16)0x0000, (fract16)0x0000, (fract16)0x0002, (fract16)0x0000, (fract16)0xFFF9, (fract16)0xFFFB, (fract16)0x0007, (fract16)0x000A, (fract16)0xFFFC, (fract16)0xFFF2, (fract16)0xFFFC, (fract16)0x000B, (fract16)0x0008, (fract16)0xFFFA, (fract16)0xFFF7, (fract16)0x0000, (fract16)0x0002, (fract16)0xFFFF, (fract16)0x0004, (fract16)0x0009, (fract16)0xFFFC, (fract16)0xFFEC, (fract16)0xFFFA, (fract16)0x0017, (fract16)0x0014, (fract16)0xFFF0, (fract16)0xFFE1, (fract16)0x0000, (fract16)0x001B, (fract16)0x000C, (fract16)0xFFF1, (fract16)0xFFF4, (fract16)0x0001, (fract16)0xFFFD, (fract16)0xFFFC, (fract16)0x0012, (fract16)0x0016, (fract16)0xFFEB, (fract16)0xFFD0, (fract16)0x0000, (fract16)0x003B, (fract16)0x001F, (fract16)0xFFD1, (fract16)0xFFC9, (fract16)0x000F, (fract16)0x0033, (fract16)0x000A, (fract16)0xFFE7, (fract16)0xFFF7, (fract16)0xFFFE, (fract16)0xFFEA, (fract16)0x0000, (fract16)0x003A, (fract16)0x0026, (fract16)0xFFBD, (fract16)0xFFA3, (fract16)0x001F, (fract16)0x007B, (fract16)0x001F, (fract16)0xFF97, (fract16)0xFFB1, (fract16)0x0031, (fract16)0x004A, (fract16)0xFFFF, (fract16)0xFFE7, (fract16)0x0003, (fract16)0xFFE3, (fract16)0xFFBF, (fract16)0x001C, (fract16)0x008D, (fract16)0x002B, (fract16)0xFF56, (fract16)0xFF6A, (fract16)0x0070, (fract16)0x00D9, (fract16)0xFFFE, (fract16)0xFF3B, (fract16)0xFFA8, (fract16)0x0065, (fract16)0x0055, (fract16)0xFFF2, (fract16)0x0008, (fract16)0x0011, (fract16)0xFF8E, (fract16)0xFF78, (fract16)0x007E, (fract16)0x0125, (fract16)0xFFFE, (fract16)0xFE8E, (fract16)0xFF38, (fract16)0x0122, (fract16)0x0158, (fract16)0xFF9E, (fract16)0xFEBA, (fract16)0xFFBC, (fract16)0x00A1, (fract16)0x0041, (fract16)0x0004, (fract16)0x0076, (fract16)0xFFFF, (fract16)0xFEA6, (fract16)0xFF15, (fract16)0x0199, (fract16)0x0242, (fract16)0xFF3E, (fract16)0xFCE7, (fract16)0xFF2E, (fract16)0x02BB, (fract16)0x021A, (fract16)0xFEA8, (fract16)0xFDDD, (fract16)0x0000, (fract16)0x00D2, (fract16)0xFFFA, (fract16)0x00C1, (fract16)0x01E7, (fract16)0xFF1B, (fract16)0xFB46, (fract16)0xFE6F, (fract16)0x066F, (fract16)0x0632, (fract16)0xFAF3, (fract16)0xF511, (fract16)0x0001, (fract16)0x0D03, (fract16)0x0729, (fract16)0xF55E, (fract16)0xF28B, (fract16)0x041A, (fract16)0x0FF4, (fract16)0x041A, (fract16)0xF28B, (fract16)0xF55E, (fract16)0x0729, (fract16)0x0D03, (fract16)0x0001, (fract16)0xF511, (fract16)0xFAF3, (fract16)0x0632, (fract16)0x066F, (fract16)0xFE6F, (fract16)0xFB46, (fract16)0xFF1B, (fract16)0x01E7, (fract16)0x00C1, (fract16)0xFFFA, (fract16)0x00D2, (fract16)0x0000, (fract16)0xFDDD, (fract16)0xFEA8, (fract16)0x021A, (fract16)0x02BB, (fract16)0xFF2E, (fract16)0xFCE7, (fract16)0xFF3E, (fract16)0x0242, (fract16)0x0199, (fract16)0xFF15, (fract16)0xFEA6, (fract16)0xFFFF, (fract16)0x0076, (fract16)0x0004, (fract16)0x0041, (fract16)0x00A1, (fract16)0xFFBC, (fract16)0xFEBA, (fract16)0xFF9E, (fract16)0x0158, (fract16)0x0122, (fract16)0xFF38, (fract16)0xFE8E, (fract16)0xFFFE, (fract16)0x0125, (fract16)0x007E, (fract16)0xFF78, (fract16)0xFF8E, (fract16)0x0011, (fract16)0x0008, (fract16)0xFFF2, (fract16)0x0055, (fract16)0x0065, (fract16)0xFFA8, (fract16)0xFF3B, (fract16)0xFFFE, (fract16)0x00D9, (fract16)0x0070, (fract16)0xFF6A, (fract16)0xFF56, (fract16)0x002B, (fract16)0x008D, (fract16)0x001C, (fract16)0xFFBF, (fract16)0xFFE3, (fract16)0x0003, (fract16)0xFFE7, (fract16)0xFFFF, (fract16)0x004A, (fract16)0x0031, (fract16)0xFFB1, (fract16)0xFF97, (fract16)0x001F, (fract16)0x007B, (fract16)0x001F, (fract16)0xFFA3, (fract16)0xFFBD, (fract16)0x0026, (fract16)0x003A, (fract16)0x0000, (fract16)0xFFEA, (fract16)0xFFFE, (fract16)0xFFF7, (fract16)0xFFE7, (fract16)0x000A, (fract16)0x0033, (fract16)0x000F, (fract16)0xFFC9, (fract16)0xFFD1, (fract16)0x001F, (fract16)0x003B, (fract16)0x0000, (fract16)0xFFD0, (fract16)0xFFEB, (fract16)0x0016, (fract16)0x0012, (fract16)0xFFFC, (fract16)0xFFFD, (fract16)0x0001, (fract16)0xFFF4, (fract16)0xFFF1, (fract16)0x000C, (fract16)0x001B, (fract16)0x0000, (fract16)0xFFE1, (fract16)0xFFF0, (fract16)0x0014, (fract16)0x0017, (fract16)0xFFFA, (fract16)0xFFEC, (fract16)0xFFFC, (fract16)0x0009, (fract16)0x0004, (fract16)0xFFFF, (fract16)0x0002, (fract16)0x0000, (fract16)0xFFF7, (fract16)0xFFFA, (fract16)0x0008, (fract16)0x000B, (fract16)0xFFFC, (fract16)0xFFF2, (fract16)0xFFFC, (fract16)0x000A, (fract16)0x0007, (fract16)0xFFFB, (fract16)0xFFF9, (fract16)0x0000, (fract16)0x0002, (fract16)0x0000, (fract16)0x0000, (fract16)0x0001, (fract16)0xFFFF, (fract16)0xFFFB, (fract16)0xFFFE, (fract16)0x0004, (fract16)0x0003, (fract16)0xFFFD, (fract16)0xFFFB, (fract16)0xFFFF, (fract16)0x0003, (fract16)0x0001, (fract16)0xFFFE, (fract16)0xFFFE, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFE, (fract16)0xFFFF, (fract16)0x0001, (fract16)0x0000, (fract16)0xFFFE, (fract16)0xFFFE, (fract16)0x0000, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF, (fract16)0x0000, (fract16)0x0000, (fract16)0xFFFF, (fract16)0xFFFF};


void io_thread(std::atomic<bool>& running){
    while(running){

        //ebben a peldaban nincs I/O

    }
}


void process_thread(std::atomic<bool>& running){

    /************************************/
    /* ESETLEGES INICIALIZALASI LEPESEK */
    /************************************/

    int i = 0;

    /************************************/
    /*        INICIALIZALAS VEGE        */
    /************************************/

    initDoneSignal.notify();

    while(running){
        bufferFullSignal.wait();
        if(!running) break;

        /************************************/
        /* IDE JON A LENYEGI JELFELDOLGOZAS */
        /************************************/

        for(int bufidx=0; bufidx<BUFFER_SIZE; bufidx++){ //ugyanugy vegezzuk a szurest, mint a nem blokkos esetben, csak most vegigiteralunk a bufferen
            int j, ix = i;
            fract16 out = 0;
            input[i] = inRightBuf[bufidx];
            for(j=0; j<N; j++) out = add_fr1x16(out, mult_fr1x16(input[ix++%N], coeffs[j]));
            i = (i==0) ? (N-1) : (i-1);
            outLeftBuf[bufidx] = inRightBuf[bufidx]; //a bal csatornan atengedjuk a bemeno jelet
            outRightBuf[bufidx] = out; //a jobb csatornan lesz a szurt jel
        }
        
        /************************************/
        /*       JELFELDOLGOZAS VEGE        */
        /************************************/

        processingDoneSignal.notify();
    }
}
