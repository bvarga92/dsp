%% AKTIV ZAJCSOKKENTES ADAPTIV ALGORITMUSOKKAL

clear all;
clc;

%% A MASODLAGOS UT IDENTIFIKACIOJA NLMS ALGORITMUSSAL
N=200;                  %mintaszam
M=10;                   %FIR modell egyutthatoszama
u=wgn(1,N,10);          %a kiadott gerjesztes (Gauss-zaj)
SB=[1 2 3 4 5]; SA=[1]; %a masodlagos ut atvitele (S=SB/SA);
y=filter(SB,SA,u);      %a mikrofon bemenete
[e,c]=nlms(1,1,M,u,y);
figure(1);
subplot(211);
plot(e);
title('A masodlagos ut identifikacioja');
xlabel('t');
ylabel('e');

%% ZAJCSOKKENTES FXLMS ALGORITMUSSAL
N=1000;                        %mintaszam (szimulacios ido)
M=300;                         %W hossza
[B,A]=ellip(6,1,60,0.3);       %az elsodleges ut atvitele (P=B/A);
x=sawtooth(2*pi*0.05*(0:N-1)); %a zavarjel (referencia)
y=filter(B,A,x);               %zavarjel az elnyomas helyen
[e,w]=fxlms(1e-5,M,SB,c,x,y);
figure(1);
subplot(212);
plot(y);
hold on;
plot(e,'r');
hold off;
title('A zavarelnyomas konvergenciaja');
xlabel('t');
ylabel('y, e');

%% A MEGTANULT ATVITEL ELLENORZESE
[tf_W,omega]=freqz(w,1,10000);
[tf_P,omega]=freqz(B,A,10000);
[tf_S,omega]=freqz(SB,1,10000);
figure(2);
plot(omega/2/pi,20*log10(abs(tf_P./tf_S)));
hold on;
plot(omega/2/pi,20*log10(abs(tf_W)),'r');
hold off;
legend('P/S','W');
title('Az identifikalt atvitel');
xlabel('f');
