# DSP2 mérések – távoktatás

Az előző mérésben használt [változathoz](https://github.com/bvarga92/dsp/tree/main/dspemul) hasonlóan a mellékelt program egy 16 bites, fixpontos DSP működését hivatott emulálni, azonban a programozási modell most más: a program több végrehajtási szálból áll, a jelfeldolgozást pedig nem mintáról mintára, hanem blokkosan végezzük. A bemenetet ezúttal is az input.dat fájlból olvassa a program, a kimenetet pedig az output.dat fájlba írja. A fájlok formátuma nem változott, a generáláshoz és a megjelenítéshez továbbra is használhatók a régi MATLAB vagy Python szkriptek. A mintavételi frekvencia továbbra is **48&nbsp;kHz**-nek tekintendő.

## Fordítás

A lefordítandó projekt az alábbi fájlokból áll:

- fr1x16.c
- fr1x16.h
- globals.hpp
- main.cpp
- Semaphore.hpp
- threads.cpp

A fordításhoz javasolt a mellékelt Makefile-t használni, de természetesen nem kötelező. Az viszont fontos, hogy a C++ kódokhoz C++11 fordítót használjunk (`-std=c++11`) multithreading támogatással (`-pthread`)!

## A jelfeldolgozó kód fejlesztése

A választott feladat megoldása során egyedül a **threads.cpp** fájlt kell szerkeszteni. Ebben két szál található: az egyik a jelfeldolgozáshoz, a másik a terminál kezeléséhez. A terminálra írni (`std::cout << ...`) csak az utóbbi szálból szabad! A szálak közötti kommunikációra megosztott változókat és szemaforokat használjunk! A Semaphore osztály a következő metódusokat tartalmazza:

```c
Semaphore s;

s.wait(); //blokkolva várakozik a jelzésre

s.wait_for_ms(t); //t milliszekundum ideig várakozik blokkolva, azután visszatér
                  //a visszatérési érték igaz, ha volt jelzés, és hamis, ha letelt az időkorlát

s.notify(); //jelzés
```

A jelfeldolgozás bufferelten történik. A bufferméret 1024 minta, ez szükség esetén a globals.hpp fejlécben módosítható. A jelfeldolgozó szál számára a bufferFullSignal nevű szemafor jelzi, hogy a bemeneti bufferekben (inLeftBuf, inRightBuf) összegyűlt 1024 új minta. Ezek alapján fel kell tölteni a kimeneti buffereket (outLeftBuf, outRightBuf), és ennek elkészültét jelezni kell a processingDoneSignal szemaforral.

Érdemes az examples könyvtárban található példák áttekintésével kezdeni a feladatot. A FIR szűrő példája jól illusztrálja a blokkos jelfeldolgozás menetét, a DTMF dekóder pedig a szálak közti kommunikációt.