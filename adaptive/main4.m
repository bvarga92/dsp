%% ADAPTIV VISSZHANGCSOKKENTES

clear all;
clc;

%% PARAMETEREK
N=1000;                        %mintak szama (szimulacios ido)
N_train=5000;                  %identifikacios ido
[B,A]=ellip(6,1,60,0.3);       %atvitel a visszhangra
M=300;                         %FIR modell egyutthatoinak szama
mu=1;                          %batorsagi tenyezo
x_train=wgn(1,N,1);            %identifikacios jel
x=sawtooth(2*pi*(0:N-1)*0.02); %az altalunk adott jel
u=sin(2*pi*(0:N-1)*0.03);      %a masik fel altal adott jel

%% AEC ALGORITMUS
[e_train,w]=nlms(mu,1,M,x_train,filter(B,A,x_train));
y=filter(B,A,x)+u;
e=y-filter(w,1,x);

%% ABRAZOLAS
figure(1);
subplot(311);
plot(x);
title('Adott jel');
ylabel('x');
subplot(312);
plot(e);
title('Vett jel');
ylabel('e');
subplot(313);
plot(y);
title('Vett jel AEC nelkul');
ylabel('y');
