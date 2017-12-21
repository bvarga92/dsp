# A torzításcsökkentés megvalósítása DSP-n

- **dc1_megfigyelo**: állapotmegfigyelő algoritmus 440 Hz-es jelre 10 komponenssel
- **dc2_megfigyelo_afa**: állapotmegfigyelő algoritmus + adaptív Fourier-analizátor
- **dc3_szabalyzo**: áramkör átvitelének identifikációja, majd linearizálása a felharmonikusok nullára szabályozásával
- **dc4_iq_szinusz_elnyomas**: nagyfrekvenciás szinuszjel nullára szabályozása IQ keveréssel

## Elrendezés a nagyfrekvenciás mérésekhez:

![1](https://raw.githubusercontent.com/bvarga92/dsp/main/distcanc/dsp/setup1.jpg)

![2](https://raw.githubusercontent.com/bvarga92/dsp/main/distcanc/dsp/setup2.jpg)

![3](https://raw.githubusercontent.com/bvarga92/dsp/main/distcanc/dsp/setup3.jpg)
