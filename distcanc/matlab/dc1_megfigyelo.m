clear all;
close all;
clc;
%-------------------- BEMENET, PARAMETEREK --------------------%
fs=48000; %mintaveteli frekvencia (Hz)
t=0:1/fs:0.003; %idotengely (s)
f0=1600; %alapharmonikus frekvenciaja (Hz)
u=1+...
   cos(2*pi*f0*t+60/180*pi)+...
   cos(2*pi*2*f0*t)+...
   1.5*cos(2*pi*3*f0*t+150/180*pi); %bemenet
N=15; %megfigyelendo harmonikusok szama (0=DC)
alpha=0.5/N; %az egyes csatornak erositesei
%-------------------------- SZAMITAS --------------------------%
y=zeros(size(t));
x=zeros(N+1,1);
e=0;
xSave=zeros(N+1,length(t));
eSave=zeros(size(t));
for ii=1:length(t)
    c=exp(1j*2*pi*f0/fs*(ii-1)*(0:N));
    y(ii)=c(1)*x(1)+2*real(c(2:end)*x(2:end));
    xSave(:,ii)=x;
    eSave(ii)=e;
    e=u(ii)-y(ii);
    x=x+alpha*e*c';   
end
%-------------- A MEGFIGYELT FOURIER-EGYUTTHATOK --------------%
for ii=1:N+1
    if ii==1
        A=abs(x(ii));
    else
        A=abs(x(ii))*2;
    end
    phi=angle(x(ii))*180/pi+(ii-1)*f0/fs*360; %1 mintanyi kesleltetest kompenzalunk
    disp(sprintf('%d. komponens amplitudoja %.3f, fazisa %.3f\xb0',ii-1,A,phi)); 
end
%------------ BEMENET, KIMENET, HIBAJEL ABRAZOLASA ------------%
figure;
plot(1000*t,u,1000*t,y,1000*t,eSave);
legend('bemenet','kimenet','hibajel');
xlabel('t [ms]');
title('A kimenet beallasa');
%---------- KONVERGENCIA ABRAZOLASA A KOMPLEX SIKON -----------%
figure;
plot3(1000*t,real(xSave),imag(xSave));
hold on;
plot3(1000*t,zeros(size(t)),zeros(size(t)),'linewidth',2,'color','k');
grid on;
xlabel('t [ms]');
ylabel('\Re(x)');
zlabel('\Im(x)');
title('A Fourier-egyutthatok beallasa');
figure;
subplot(121);
plot(1000*t,abs(xSave));
xlabel('t [ms]');
ylabel('|x|');
title('Amplitudok beallasa');
subplot(122);
plot(real(xSave'),imag(-xSave')); %a ' operator konjugal is!
grid on;
xlabel('\Re(x)');
ylabel('\Im(x)');
title('Fazisok beallasa');
axis equal;
