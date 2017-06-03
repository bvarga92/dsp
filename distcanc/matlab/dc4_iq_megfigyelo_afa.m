% KOMPLEX JELEN MUKODO MEGFIGYELO ES ADAPTIV FOURIER-ANALIZATOR
% A lekevert jel idealis esetben egy DC komponens es egy szinusz osszege.
% Ha viszont nem sikerul tokeletesen eltalalni a vivofrekvenciat, akkor
% a vivo nem pontosan 0 frekvenciara kerul, a spektrum nem szimmetrikus,
% az eredo jel komplex. Az RDFT ebben az esetben is mukodik.

clear all;
clc;
%-------------------- BEMENET, PARAMETEREK --------------------%
fs=4800; %mintaveteli frekvencia
f_am=1000; %modulalojel frekvenciaja
f_dc=10; %frekvenciahiba (ez van DC helyett)
M=48000; %mintaszam
t=(0:M-1)/fs; %idovektor (s)
u=2*exp(1j*1)*exp(1j*2*pi*(-f_am+f_dc)*t)+... %egyik oldalsav
  3*exp(1j*0)*exp(1j*2*pi*(0+f_dc)*t)+... %vivo (nevlegesen DC)
  2*exp(-1j*1)*exp(1j*2*pi*(f_am+f_dc)*t); %masik oldalsav
alpha_obs=0.001; %megfigyelo batorsagi tenyezoje
alpha_afa=1; %AFA batorsagi tenyezoje
%-------------------------- SZAMITAS --------------------------%
y=zeros(size(t)); %a jel becsloje
x=zeros(3,1); %itt szeretnenk eloallitani a Fourier-egyutthatokat
xSave=zeros(3,M);
f=[-f_am 0 f_am]; %a csatornak frekvenciait a nevlegesre inicializaljuk
df=zeros(M+1,1); %frekvenciahiba
for ii=1:M
    c=exp(1j*2*pi*(f+df(ii))*t(ii));
    y(ii)=c*x;
    xSave(:,ii)=x;
    e=u(ii)-y(ii);
    x=x+alpha_obs*e*c';
    df(ii+1)=df(ii)+alpha_afa*wrapToPi(angle(x(2)/xSave(2,ii)));
end
%------------------------- ABRAZOLAS --------------------------%
fprintf('Frekvenciahiba: df= %.3f Hz\nA megfigyelt egyutthatok:\n',df(end));
for ii=1:3
    fprintf('\tx%d= %.1f*exp(%.1f*j)\n',ii,abs(x(ii,end)),wrapToPi(angle(x(ii,end))));
end

figure(1);
plot((-M/2:M/2-1)*fs/M/1000,abs(fftshift(fft(u)/M)));
title('Komplex jel spektruma');
xlabel('f [kHz]');
ylabel('X');

figure(2);
plot((0:M-1)/fs,abs(xSave.'));
title('Egyutthatok konvergenciaja');
xlabel('t [s]');
ylabel('X');

figure(3);
plot((0:M)/fs,df);
title('Frekvenciameres konvergenciaja');
xlabel('t [s]');
ylabel('\Deltaf');
