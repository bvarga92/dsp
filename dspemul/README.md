# DSP1 mérések – távoktatás

A mellékelt C nyelvű program a laborban használt 16 bites, fixpontos DSP működését és a mérések során megismert programozási modellt emulálja. A valóságban az A/D konverter által mintavételezett jelet itt az input.dat fájlból olvassa a program, D/A konverter híján pedig az output.dat fájlban állítja elő a kimeneti jeleket. A fájlok tartalma ezután a mellékelt MATLAB vagy Python szkriptek segítségével ábrázolható és megszólaltatható.

## A jelfeldolgozó kód fejlesztése

A laboratóriumi mérésekhez hasonlóan a feladat ezúttal is a process.c fájl kiegészítése. A fájlban található függvény a bemeneti jelpár minden mintájára külön-külön meghívódik, a mintavételezési frekvencia **48&nbsp;kHz**-nek tekintendő. Amennyiben a main.exe programot tartalmazó könyvtárban létezik az input.dat fájl, a program addig fut, amíg a teljes bemeneti jelet be nem olvasta. Egyes feladatoknál nincsen szükség bemeneti jelre (pl. dallamgenerátor), ekkor parancssori argumentumként adható meg, hogy a program hány ütemig fusson. Például ha 10 másodperc hosszú kimeneti jelet szeretnénk generálni, akkor a `main.exe 480000` paranccsal indítsuk a programot.

Tudnivalók:

- A program fordításához használható a mellékelt Makefile, de nem kötelező, mindenki használja a kedvenc fejlesztőkörnyezetét. A Makefile Windowshoz készült, de egyszerűen módosítható Linuxra (main.exe helyett main, del helyett rm).

- Az fr1x16.h fájlban látható, hogy a *fract16* változótípust kezelő függvények közül melyek használhatók a programban. Ezek többségének működése megegyezik a Blackfin C fordító dokumentációjában leírtaknak, az esetleges eltéréseket kommentben jeleztem (pl. az FFT függvény nem igényel előre feltöltött twiddle table-t).

- Fixpontos literáloknál az *r16* suffix nem használható, helyette válasszuk a konverziós függvényt vagy az egész számként értelmezett értéket:

```c
fract16 x = 0.5r16;             // nem jó
fract16 x = float_to_fr16(0.5); // jó
fract16 x = 16384;              // jó
```

- A section kulcsszó nem használható, nem különböztetünk meg memóriabankokat.

- A be- és kimenő minták 16 bitesek, tehát nem kell 8 bites eltolást alkalmazni, mint a laborban a 24 bites DAC miatt.

- Célszerű az examples könyvtárban található példák áttekintésével kezdeni a feladatot.

## A ki- és bemeneti jelek megjelenítése

Az input.dat és output.dat fájlok sorai szóközzel elválasztva, decimális, egész számként értelmezve tartalmazzák a bal és a jobb csatorna mintáit. Ilyen bemeneti jelek előállításához használhatjuk a generate_input szkriptet, a jelek megjelenítéséhez és megszólaltatásához pedig a show_signal nevű szkriptet. Mindkettő rendelkezésre áll MATLAB és Python nyelven is. A Python verzióhoz a numpy, scipy, matplotlib, simpleaudio modulok szükségesek, a pip csomagkezelővel könnyedén telepíthető mind.
