# Digitális jelfeldolgozással kapcsolatos projektek

- **adaptive**: adaptív szűrőalgoritmusok (LMS, NLMS, FxLMS) szimulációja MATLAB-ban
- **anc**: aktív zajcsökkentéssel (Active Noise Control) kapcsolatos anyagok
- **combfilter**: fésűszűrőn alapuló késleltetőeffektek (pl. visszhang, slapback) ADSP-21364 jelfeldolgozó processzorral
- **distcanc**: aktív torzításcsökkentés jelfeldolgozó processzorral megvalósított szabályozási kör segítségével
- **ekg**: EKG jel feldolgozása, QRS komplexek detektálása Pan-Tompkins-algoritmussal
- **fft_scope**: jel spektrumának megjelenítése oszcilloszkópon ADSP-21364 DSP-vel
- **fir_gpu_cuda**: FIR szűrés megvalósítása GPU-gyorsított konvolúcióval (CUDA)
- **fir_gpu_opencl**: FIR szűrés megvalósítása GPU-gyorsított konvolúcióval (OpenCL)
- **fir_sharc**: FIR szűrő ADSP-21364 DSP-vel
- **fsk_decoder**: frekvenciabillentyűzéssel kódolt karakterfolyam dekódolása
- **interp**: interpolációs módszerek összehasonlítása (lineáris, Lagrange, LS, Fourier, Hermite, spline, aluláteresztő szűrő)
- **reverb**: visszacsatolt késleltetőhálózaton alapuló zengető effekt ADSP-21364 jelfeldolgozó processzorral ([videó](https://www.youtube.com/watch?v=-eSbS0YqtKs&list=PL9_VlVdB8s882QMHiqJlDpJeKWxwP5CIG))
- **stm32f7_scope**: audio bemenet és a spektrum megjelenítése az STM32F746-Discovery kártyán ([videó](https://www.youtube.com/watch?v=P65fDKEUeow&list=PL9_VlVdB8s882QMHiqJlDpJeKWxwP5CIG))
- **fft.c**: gyors Fourier-transzformáció nonrekurzív implementációja C nyelven + Blackman-ablak
- **fft_windowing.m** és **fft_windowing.ipynb**: szimulációk a nem koherens mintavételezés és az ablakozás hatásáról
- **filters.m**: IIR és FIR szűrőapproximációk összehasonlítása (Butterworth, Csebisev, inverz Csebisev, elliptikus, frekvenciamintavételezés, Remez-algoritmus)
- **ola.m**: hosszú jel FIR szűrése a frekvenciatartományban Overlap & Add algoritmussal
- **remezfir.m**: a Remez-módszer manuális implementációja aluláteresző FIR szűrő tervezésére (Parks-McClellan-algoritmus)
