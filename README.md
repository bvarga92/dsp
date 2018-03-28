# Digitális jelfeldolgozással kapcsolatos projektek

- **adaptive**: adaptív szűrőalgoritmusok (LMS, NLMS, FxLMS) szimulációja MATLAB-ban
- **anc**: aktív zajcsökkentéssel (Active Noise Control) kapcsolatos anyagok
- **distcanc**: aktív torzításcsökkentés jelfeldolgozó processzorral megvalósított szabályozási kör segítségével
- **efm32_fir**: jelfeldolgozás EFM32GG-STK3700 fejlesztőkártyán (fixpontos FIR szűrő)
- **efm32_iir**: jelfeldolgozás EFM32GG-STK3700 fejlesztőkártyán (elsőfokú IIR szűrő)
- **ekg**: EKG jel feldolgozása, QRS komplexek detektálása Pan-Tompkins-algoritmussal
- **gpu_fir_cuda**: FIR szűrés megvalósítása GPU-gyorsított konvolúcióval (CUDA)
- **gpu_fir_opencl**: FIR szűrés megvalósítása GPU-gyorsított konvolúcióval (OpenCL)
- **interp**: interpolációs módszerek összehasonlítása (lineáris, Lagrange, LS, Fourier, Hermite, spline, aluláteresztő szűrő)
- **sharc_combfilter**: fésűszűrőn alapuló késleltetőeffektek (pl. visszhang, slapback) ADSP-21364 jelfeldolgozó processzorral
- **sharc_fft_scope**: jel spektrumának megjelenítése oszcilloszkópon ADSP-21364 DSP-vel
- **sharc_fir**: FIR szűrő ADSP-21364 DSP-vel
- **sharc_fsk_decoder**: frekvenciabillentyűzéssel kódolt karakterfolyam dekódolása
- **sharc_reverb**: visszacsatolt késleltetőhálózaton alapuló zengető effekt ADSP-21364 jelfeldolgozó processzorral ([videó](https://www.youtube.com/watch?v=-eSbS0YqtKs&list=PL9_VlVdB8s882QMHiqJlDpJeKWxwP5CIG))
- **stm32f746_scope**: audio bemenet és a spektrum megjelenítése az STM32F746-Discovery kártyán ([videó](https://www.youtube.com/watch?v=P65fDKEUeow&list=PL9_VlVdB8s882QMHiqJlDpJeKWxwP5CIG))
- **trigger**: tranziens jel detektálása nagyon kis jel/zaj viszony mellett (Simulink modell)
- **butterworth.m**: IIR sávszűrő manuális tervezése Butterworth-approximációval
- **emd.m**: Empirical Mode Decomposition
- **fft.c**: gyors Fourier-transzformáció nonrekurzív implementációja C nyelven + Blackman-ablak
- **fft_windowing.m** és **fft_windowing.ipynb**: szimulációk a nem koherens mintavételezés és az ablakozás hatásáról
- **filters.m**: IIR és FIR szűrőapproximációk összehasonlítása (Butterworth, Csebisev, inverz Csebisev, elliptikus, frekvenciamintavételezés, Remez-algoritmus)
- **hilbert_fir.m**: burkológörbe előállítása FIR szűrőkkel végzett Hilbert-transzformációval
- **ident_spline.m**: statikus nemlinearitás adaptív identifikációja (Catmull-Rom-spline illesztése LMS algoritmussal)
- **kalman.m**: megfigyelési zaj kiszűrése Kalman-szűrővel
- **kalman_fusion.m**: szenzorfúzió Kalman-szűrővel
- **kurtogram.m**: tranziens zavarjel detektálása kurtogrammal
- **meda_momeda.m**: periodikus zavarjel detektálása Minimum Entropy Deconvolution algoritmussal
- **ola.m**: hosszú jel FIR szűrése a frekvenciatartományban Overlap & Add algoritmussal
- **peaks_ampd.m**: lokális szélsőértékek keresése Automatic Multiscale-Based Peak Detection algoritmussal
- **peaks_simple.m**: lokális szélsőértékek keresése
- **remezfir.m**: a Remez-módszer manuális implementációja aluláteresző FIR szűrő tervezésére (Parks-McClellan-algoritmus)
- **spectral_kurtosis.m**: spektrális kurtózis számítása és alkalmazása jelkomponensek detektálására
- **wavelet_processing.m**: wavelet alapú feature extraction
