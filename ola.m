clear all;
clc;

%% Parameterek
N=100; %FIR szuro hossza
L=10000; %a szurendo jel hossza
M=500; %blokkmeret (legyen L osztoja)

%% Bemeneti jel (fs=1)
t=0:(L-1);
x=sin(2*pi*0.1*t)+2*sin(2*pi*0.5*t+1);
subplot(221);
plot(t(800:900),x(800:900));
axis([t(800) t(900) -3 3])
title('Eredeti jel');
xlabel('t');
ylabel('x');

%% Alulatereszto FIR szuro
h=firpm(N-1,[0 0.2 0.3 1],[1 1 0 0]);
[H,W]=freqz(h,1,1000);
subplot(212);
plot(W/pi,20*log10(abs(H)));
axis([0 1 -150 50]);
grid on;
title('A szuro atvitele');
xlabel('f');
ylabel('|H| [dB]');

%% OLA algoritmus
H=fft([h zeros(1,M)]); %az impulzusvalasz M+N pontos spektruma
y=zeros(1,L+N);
for ii=0:(L/M-1)
    X=fft([x((ii*M+1):((ii+1)*M)) zeros(1,N)]); %az ii-edik blokk M+N pontos spektruma
    y((ii*M+1):(ii*M+M+N))=y((ii*M+1):(ii*M+M+N))+real(ifft(H.*X)); %real csak numerikus okokbol
end
error=max(abs(filter(h,1,x)-y(1:L))) %ellenorzes

%% Kimeneti jel
subplot(222);
plot(t(800:900),y(800:900));
axis([t(800) t(900) -3 3])
title('Szurt jel');
xlabel('t');
ylabel('y');
