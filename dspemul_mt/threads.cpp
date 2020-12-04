#include "globals.hpp"


void io_thread(std::atomic<bool>& running){
    while(running){

        /* minden kiiras ebben a szalban tortenjen */

    }
}


void process_thread(std::atomic<bool>& running){

    /************************************/
    /* ESETLEGES INICIALIZALASI LEPESEK */
    /************************************/



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

        for(int i=0; i<BUFFER_SIZE; i++){
            outLeftBuf[i] = inLeftBuf[i];
            outRightBuf[i] = inRightBuf[i];
        }
        
        /************************************/
        /*       JELFELDOLGOZAS VEGE        */
        /************************************/

        processingDoneSignal.notify();
    }
}
