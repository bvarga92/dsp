%% RENDSZERIDENTIFIKACIO ADAPTIV SZUROKKEL

clear all;
clc;

%% PARAMETEREK
N=10000;                  %mintak szama
M=300;                    %FIR modell egyutthatoinak szama
P=10;                     %a gerjeszto zaj teljesitmenye
%B=[1 2 3 4 5]; A=[1];    %identifikalando rendszer (FIR);
[B,A]=ellip(6,1,60,0.3);  %identifikalando rendszer (IIR);

%% IDENTIFIKACIO
u=wgn(1,N,P);                    %gerjesztes (Gauss-zaj)
y=filter(B,A,u);                 %a rendszer kimenete
[e_lms,w_lms]=lms(0.0002,M,u,y); %identifikacio LMS algoritmussal
[e_nlms,w_nlms]=nlms(1,1,M,u,y); %identifikacio NLMS algoritmussal

%% ABRAZOLAS
figure(1);
subplot(211);
plot(e_lms);
title('LMS');
xlabel('t');
ylabel('e');
subplot(212);
plot(e_nlms);
title('NLMS');
xlabel('t');
ylabel('e');

[H,W]=freqz(B,A,10000);
[H_LMS,W_LMS]=freqz(w_lms,1,10000);
[H_NLMS,W_NLMS]=freqz(w_nlms,1,10000);
figure(2);
plot(W/2/pi,20*log10(abs(H)));
hold on;
plot(W_LMS/2/pi,20*log10(abs(H_LMS)),'r');
plot(W_NLMS/2/pi,20*log10(abs(H_NLMS)),'g');
hold off;
legend('eredeti','LMS','NLMS');
title(sprintf('FIR tap szam: %d',M));
xlabel('f');
ylabel('H [dB]');
