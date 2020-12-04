#ifndef _GLOBALS_HPP_
#define _GLOBALS_HPP_

#include <atomic>
#include "Semaphore.hpp"
extern "C" {
    #include "fr1x16.h"
}

#define BUFFER_SIZE 1024 //buffermeret

extern fract16 inLeftBuf[BUFFER_SIZE], inRightBuf[BUFFER_SIZE], outLeftBuf[BUFFER_SIZE], outRightBuf[BUFFER_SIZE];  //be- es kimeneti bufferek

extern Semaphore initDoneSignal; //inicializalas befejezeset jelzo szemafor
extern Semaphore bufferFullSignal; //a bemeneti bufferek megteltet jelzo szemafor
extern Semaphore processingDoneSignal; //a jelfeldolgozas elkeszultet jelzo szemafor (vagyis a kimeneti bufferek ervenyes adatot tartalmaznak)

void io_thread(std::atomic<bool>& running); //szal a terminal kezelesehez
void process_thread(std::atomic<bool>& running); //szal a jelfeldolgozashoz

#endif
