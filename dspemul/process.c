#include "fr1x16.h"

extern fract16 inLeft, inRight, outLeft, outRight; //a be- es kimeno sztereo jelparok

void init(){
    /* ez a fuggveny egyszer, a program indulasakor fut le, az esetlegesen
      szukseges inicializacios feladatok elvegezhetok itt*/
}

void process(){

    /* ide jon a jelfeldolgozo rutin */
    outLeft = inLeft;
    outRight = inRight;

}
