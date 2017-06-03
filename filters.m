clear all;
clc;

%% Alulatereszto szuro parameterei
Wp=0.3; %ateresztosav vege (1 -> fs/2)
Ws=0.5; %zarosav kezdete (1 -> fs/2)
Rp=1; %maximalis ateresztosavi ingadozas (dB)
Rs=60; %minimalis zarosavi elnyomas (dB)

%% Butterworth-approximacio
[N W0]=buttord(Wp,Ws,Rp,Rs);
[B A]=butter(N,W0);
[H W]=freqz(B,A,1000);
subplot(231);
plot(W/pi,20*log10(abs(H)));
grid on;
axis([0 1 -120 20]);
title(sprintf('Butterworth (%d)',N));

%% Csebisev-approximacio
[N W0]=cheb1ord(Wp,Ws,Rp,Rs);
[B A]=cheby1(N,Rp,W0);
[H W]=freqz(B,A,1000);
subplot(232);
plot(W/pi,20*log10(abs(H)));
grid on;
axis([0 1 -120 20]);
title(sprintf('Csebisev (%d)',N));

%% Inverz Csebisev-approximacio
[N W0]=cheb2ord(Wp,Ws,Rp,Rs);
[B A]=cheby2(N,Rs,W0);
[H W]=freqz(B,A,1000);
subplot(233);
plot(W/pi,20*log10(abs(H)));
grid on;
axis([0 1 -120 20]);
title(sprintf('Inverz Csebisev (%d)',N));

%% Elliptikus (Cauer-) approximacio
[N W0]=ellipord(Wp,Ws,Rp,Rs);
[B A]=ellip(N,Rp,Rs,W0);
[H W]=freqz(B,A,1000);
subplot(234);
plot(W/pi,20*log10(abs(H)));
grid on;
axis([0 1 -120 20]);
title(sprintf('Elliptikus (%d)',N));

%% FIR szuro frekvenciamintavetelezessel-ablakozassal
N=270;
h=fir2(N,[0 Wp Ws 1],[1 1 0 0]);
[H W]=freqz(h,1,1000);
subplot(235);
plot(W/pi,20*log10(abs(H)));
grid on;
axis([0 1 -120 20]);
title(sprintf('FIR ablakozassal (%d)',N));

%% FIR szuro Remez-algoritmussal
N=35;
h=firpm(N,[0 Wp Ws 1],[1 1 0 0]);
[H W]=freqz(h,1,1000);
subplot(236);
plot(W/pi,20*log10(abs(H)));
grid on;
axis([0 1 -120 20]);
title(sprintf('FIR Remez (%d)',N));
