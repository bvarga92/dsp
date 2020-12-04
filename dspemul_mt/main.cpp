#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#include "globals.hpp"

using namespace std;

fract16 inLeftBuf[BUFFER_SIZE], inRightBuf[BUFFER_SIZE], outLeftBuf[BUFFER_SIZE], outRightBuf[BUFFER_SIZE];
Semaphore initDoneSignal, bufferFullSignal, processingDoneSignal;

int main(int argc, char *argv[]){
    ifstream ifs("input.dat");
    if(!ifs.good()){
        cout << "ERROR: Cannot access input.dat. If no input file is supplied, then the number of simulation steps must be given as a command line argument." << endl << endl;
        return 1;
    }
    ofstream ofs("output.dat");
    if(!ofs.good()){
        cout << "ERROR: Cannot create output.dat." << endl << endl;
        return 1;
    }

    atomic<bool> running{true};
    thread t1(process_thread, ref(running));
    thread t2(io_thread, ref(running));
    initDoneSignal.wait();

    fract16 inLeft, inRight;
    int i = 0;
    while(ifs >> inLeft >> inRight){
        inLeftBuf[i] = inLeft;
        inRightBuf[i] = inRight;
        i++;
        if(i == BUFFER_SIZE){
            i = 0;
            bufferFullSignal.notify();
            processingDoneSignal.wait();
            for(int j=0; j<BUFFER_SIZE; j++)
                ofs << outLeftBuf[j] << ' ' << outRightBuf[j] << endl;
        }
    }

    ifs.close();
    ofs.close();
    running = false;
    bufferFullSignal.notify();
    t1.join();
    t2.join();
    return 0;
}
