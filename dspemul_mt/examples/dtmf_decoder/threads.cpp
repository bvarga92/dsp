#include "globals.hpp"
#include <iostream>

#define N 301  //szuroegyutthatok szama

fract16 input[N];
const fract16 coeffsLPF[N] = {-210,210,137,94,68,53,44,38,34,30,27,23,19,15,10,6,0,-5,-10,-16,-21,-26,-31,-35,-39,-42,-45,-46,-47,-47,-45,-43,-40,-36,-30,-25,-18,-11,-3,5,13,22,30,38,45,52,58,63,67,69,71,71,69,66,62,56,49,40,30,20,8,-4,-16,-28,-41,-53,-64,-75,-84,-92,-99,-103,-106,-107,-105,-102,-96,-88,-77,-65,-51,-36,-18,0,18,38,57,76,94,111,126,140,151,160,166,168,168,164,156,145,130,112,91,66,40,11,-20,-52,-85,-117,-150,-181,-210,-236,-259,-278,-293,-303,-307,-305,-297,-282,-259,-230,-193,-150,-99,-42,21,91,165,244,327,414,502,592,681,771,859,944,1025,1102,1174,1239,1298,1348,1391,1424,1448,1463,1468,1463,1448,1424,1391,1348,1298,1239,1174,1102,1025,944,859,771,681,592,502,414,327,244,165,91,21,-42,-99,-150,-193,-230,-259,-282,-297,-305,-307,-303,-293,-278,-259,-236,-210,-181,-150,-117,-85,-52,-20,11,40,66,91,112,130,145,156,164,168,168,166,160,151,140,126,111,94,76,57,38,18,0,-18,-36,-51,-65,-77,-88,-96,-102,-105,-107,-106,-103,-99,-92,-84,-75,-64,-53,-41,-28,-16,-4,8,20,30,40,49,56,62,66,69,71,71,69,67,63,58,52,45,38,30,22,13,5,-3,-11,-18,-25,-30,-36,-40,-43,-45,-47,-47,-46,-45,-42,-39,-35,-31,-26,-21,-16,-10,-5,0,6,10,15,19,23,27,30,34,38,44,53,68,94,137,210,-210};  //alulatereszto szuro 1075 Hz-re
const fract16 coeffsHPF[N] = {210,-210,-137,-94,-68,-53,-44,-38,-34,-30,-27,-23,-19,-15,-10,-6,0,5,10,16,21,26,31,35,39,42,45,46,47,47,45,43,40,36,30,25,18,11,3,-5,-13,-22,-30,-38,-45,-52,-58,-63,-67,-69,-71,-71,-69,-66,-62,-56,-49,-40,-30,-20,-8,4,16,28,41,53,64,75,84,92,99,103,106,107,105,102,96,88,77,65,51,36,18,0,-18,-38,-57,-76,-94,-111,-126,-140,-151,-160,-166,-168,-168,-164,-156,-145,-130,-112,-91,-66,-40,-11,20,52,85,117,150,181,210,236,259,278,293,303,307,305,297,282,259,230,193,150,99,42,-21,-91,-165,-244,-327,-414,-502,-592,-681,-771,-859,-944,-1025,-1102,-1174,-1239,-1298,-1348,-1391,-1424,-1448,-1463,31300,-1463,-1448,-1424,-1391,-1348,-1298,-1239,-1174,-1102,-1025,-944,-859,-771,-681,-592,-502,-414,-327,-244,-165,-91,-21,42,99,150,193,230,259,282,297,305,307,303,293,278,259,236,210,181,150,117,85,52,20,-11,-40,-66,-91,-112,-130,-145,-156,-164,-168,-168,-166,-160,-151,-140,-126,-111,-94,-76,-57,-38,-18,0,18,36,51,65,77,88,96,102,105,107,106,103,99,92,84,75,64,53,41,28,16,4,-8,-20,-30,-40,-49,-56,-62,-66,-69,-71,-71,-69,-67,-63,-58,-52,-45,-38,-30,-22,-13,-5,3,11,18,25,30,36,40,43,45,47,47,46,45,42,39,35,31,26,21,16,10,5,0,-6,-10,-15,-19,-23,-27,-30,-34,-38,-44,-53,-68,-94,-137,-210,210}; //felulatereszto szuro 1075 Hz-re
const char dtmfTable[4][4] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}}; //dekoder matrix

char symbol; //a vett szimbolum
Semaphore symbolReceivedSignal; //szimbolum vetelet jelzo szemafor


void io_thread(std::atomic<bool>& running){
    while(running){
        if(symbolReceivedSignal.wait_for_ms(100)) //varunk a masik szal jelzesere
            std::cout << symbol; //kiirjuk a szimbolumot
    }
}


void process_thread(std::atomic<bool>& running){

    /************************************/
    /* ESETLEGES INICIALIZALASI LEPESEK */
    /************************************/

    int i = 0;
    bool newSymbolRecvd = false;

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

        /* vegigiteralunk a bufferen */
        int levelLPF = 0, levelHPF = 0, signLPF_prev = 0, signHPF_prev = 0, zcrossLPF = 0, zcrossHPF = 0;
        for(int bufidx=0; bufidx<BUFFER_SIZE; bufidx++){
            /* alul- es felulatereszto szurest vegzunk a jelen */
            int j, ix = i;
            fract16 outLPF = 0, outHPF = 0;
            input[i] = inRightBuf[bufidx];
            for(j=0; j<N; j++){
                outLPF = add_fr1x16(outLPF, mult_fr1x16(input[ix%N], coeffsLPF[j]));
                outHPF = add_fr1x16(outHPF, mult_fr1x16(input[ix%N], coeffsHPF[j]));
                ix++;
            }
            i = (i==0) ? (N-1) : (i-1);
            outLeftBuf[bufidx] = outLPF; //a bal csatornan kiadjuk az alacsony frekvencias reszt
            outRightBuf[bufidx] = outHPF; //a jobb csatornan kiadjuk a magas frekvencias reszt

            /* megmerjuk a jelszintet */
            if(abs_fr1x16(outLPF) > 6554) levelLPF++;
            if(abs_fr1x16(outHPF) > 6554) levelHPF++;

            /* fapados frekvenciameres: megszamoljuk a nullatmeneteket */
            int sign = (outLPF & (fract16)0x8000) ? 0 : 1;
            if(bufidx > 0 && sign != signLPF_prev) zcrossLPF++;
            signLPF_prev = sign;
            sign = (outHPF & (fract16)0x8000) ? 0 : 1;
            if(bufidx > 0 && sign != signHPF_prev) zcrossHPF++;
            signHPF_prev = sign;
        }
    
        /* eldontjuk, hogy van-e DTMF jeldarab a bufferben, es ha igen, akkor milyen */
        int fLow = -1, fHigh = -1;
        if(levelLPF > 600){ //detektaltunk alacsony frekvencias jelet
            if(zcrossLPF >= 17 && zcrossLPF <= 19) //425 Hz szabad jelzes
                ;
            else if(zcrossLPF >= 28 && zcrossLPF <= 31) //697 Hz
                fLow = 0;
            else if(zcrossLPF >= 32 && zcrossLPF <= 34) //770 Hz
                fLow = 1;
            else if(zcrossLPF >= 35 && zcrossLPF <= 38) //852 Hz
                fLow = 2;
            else if(zcrossLPF >= 39 && zcrossLPF <= 44) //941 Hz
                fLow = 3;
        }
        if(levelHPF > 600){ //detektaltunk magas frekvencias jelet
            if(zcrossHPF >= 46 && zcrossHPF <= 53) //1209 Hz
                fHigh = 0;
            else if(zcrossHPF >= 54 && zcrossHPF <= 59) //1336 Hz
                fHigh = 1;
            else if(zcrossHPF >= 60 && zcrossHPF <= 66) //1477 Hz
                fHigh = 2;
            else if(zcrossHPF >= 67 && zcrossHPF <= 73) //1633 Hz
                fHigh = 3;
        }

        /* szimbolumok detektalasa */
        if(fLow > -1 && fHigh > -1){
            symbol = dtmfTable[fLow][fHigh];
            if(!newSymbolRecvd) symbolReceivedSignal.notify(); //jelzunk a masik szalnak, hogy irja ki a szimbolumot
            newSymbolRecvd = true;
        }
        else{
            newSymbolRecvd = false;
        }

        /************************************/
        /*       JELFELDOLGOZAS VEGE        */
        /************************************/

        processingDoneSignal.notify();
    }
}
