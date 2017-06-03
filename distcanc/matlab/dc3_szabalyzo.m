clear all;
clc;
%-------------------- BEMENET, PARAMETEREK --------------------%
sum_on_input=0; %0: beavatkozas a kimeneten, 1: beavatkozas a bemeneten
fs=48000; %mintaveteli frekvencia (Hz)
t=0:1/fs:0.05; %idotengely (s)
f0=1600; %alapharmonikus frekvenciaja (Hz)
in=0.8*cos(2*pi*f0*t+60/180*pi); %bemenet (tiszta szinusz)
N=15; %megfigyelendo harmonikusok szama (0=DC)
alpha=0.5/N; %az egyes rezonatorcsatornak erositesei
beta=0.01./exp(-j*2*pi*(0:N)'*f0/fs); %erositesek a szabalyzo csatornaiban
transfer=@(x)(atan(x*4)); %a nemlinearis rendszer karakterisztikaja
%-------------------------- SZAMITAS --------------------------%
if(sum_on_input)
    disp('Beavatkozas a bemeneten.');
else
    disp('Beavatkozas a kimeneten.');
end
out=zeros(size(t));
x=zeros(N+1,1);
p=zeros(N+1,1);
eSave=zeros(size(t));
r=0;
e=0;
for ii=1:length(t)
    if(sum_on_input)
        out(ii)=transfer(in(ii)-r);
    else
        out(ii)=transfer(in(ii))-r;
    end
    c=exp(j*2*pi*f0/fs*(ii-1)*(0:N));
    y=c(1)*x(1)+2*real(c(2:end)*x(2:end));
    r=c(1)*p(1)+2*real(c(2:end)*p(2:end));
    eSave(ii)=e;
    e=out(ii)-y;
    x=x+alpha*e*c';
    if(t(ii)>0.01) %10 ms utan kapcsoljuk be a szabalyzot
        p=p+beta.*x;
        p(2)=0; %az alapharmonikust nem szabalyozzuk nullara
    end
end
%------------------------- ABRAZOLAS --------------------------%
figure(1)
subplot(121);
plot(-2:0.01:2,transfer(-2:0.01:2));
axis([-1 1 -2 2]);
grid on;
xlabel('x');
ylabel('y');
title('Nemlinearis rendszer karakterisztikaja');

subplot(322);
plot(1000*t,in);
axis([0 max(1000*t) -2 2]);
grid on;
xlabel('t [ms]');
title('Bemenet');

subplot(324);
plot(1000*t,out);
hold on;
plot([10 10],[-2 2],'r');
hold off;
axis([0 max(1000*t) -2 2]);
grid on;
xlabel('t [ms]');
title('Kimenet');

subplot(326);
plot(1000*t,eSave);
hold on;
plot([10 10],[-3 3],'r');
hold off;
axis([0 max(1000*t) -2 2]);
grid on;
xlabel('t [ms]');
title('Megfigyelo hibajele');

figure(2);
f=0:fs/479:fs/2-fs/479;
OUT1=fft(out(1:479))/479;
OUT2=fft(out(end-478:end))/479;
plot(f/1000,20*log10(abs(OUT1(1:length(f)))));
hold on;
plot(f/1000,20*log10(abs(OUT2(1:length(f)))),'r');
hold off;
xlim([0 max(f/1000)]);
legend('szabalyzo nelkul','szabalyozva');
xlabel('f [kHz]');
ylabel('dB');
title('A torz es a tisztitott spektrum');
