clear all;
clc;

%% BEMENET, PARAMETEREK
fs=48000;              %mintaveteli frekvencia (Hz)
f0=480;                %a jel nevleges frekvenciaja (kezdeti becsles)
fe=30;                 %frekvenciahiba
t=0:1/fs:0.1;          %idovektor (s)
u=cos(2*pi*(f0+fe)*t); %bemeno jel (tiszta szinusz)
N=2;                   %megfigyelt harmonikusok szama
alpha_obs=0.005/N;     %megfigyelo batorsagi tenyezoje
alpha_afa=50;          %AFA batorsagi tenyezoje

%% SZAMITAS
y=zeros(size(t)); %jel becsloje
x=zeros(N+1,1); %Fourier-egyutthatok
e=zeros(size(t)); %hibajel
xSave=zeros(N+1,length(t));
f=f0;
for ii=1:length(t)
    c=exp(1j*2*pi*f/fs*(ii-1)*(0:N));
    y(ii)=c(1)*x(1)+2*real(c(2:end)*x(2:end));
    xSave(:,ii)=x; 
    e(ii)=u(ii)-y(ii);
    x=x+alpha_obs*e(ii)*c';
    f=f+alpha_afa*wrapToPi(angle(x(2)/xSave(2,ii)));
end

%% ABRAZOLAS
disp(sprintf('Nevleges: %.2f Hz\nMert:     %.2f Hz\nValos:    %.2f Hz',f0,f,f0+fe));

figure(1);
subplot(211);
plot(1000*t,u,'g');
hold on;
plot(1000*t,y,'b');
plot(1000*t,e,'r');
hold off;
xlim([min(1000*t) max(1000*t)]);
legend('be','ki','hiba');
xlabel('t [ms]');
title('A kimenet beallasa');

subplot(212);
plot(1000*t,unwrap((angle(xSave(2,:))))/pi*180);
xlim([min(1000*t) max(1000*t)]);
xlabel('t [ms]');
ylabel('\phi [fok]');
title('A fazis beallasa');

figure(2);
plot3(1000*t,real(xSave(2,:)),imag(xSave(2,:)));
xlim([min(1000*t) max(1000*t)]);
xlabel('t [ms]');
ylabel('\Re(x)');
zlabel('\Im(x)');
title('A fazor beallasa');
grid on;
