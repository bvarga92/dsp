clear all;
clc;

COHERENT=0; %koherens (1) vagy nem koherens (0) mintavetelezes
WINDOW_FUNCTION=2; %ablakfuggveny: flat-top (0), Hanning (1), rect (2)

fs=1800;
N=40;
Nsin=N*150;
df=fs/N;
if COHERENT==1
    f=6*df;
    disp('KOHERENS MINTAVETELEZES');
else
    f=6.5*df;
    disp('NEM KOHERENS MINTAVETELEZES');
end
if WINDOW_FUNCTION==0
    win=[zeros(1,(Nsin-N)/2) flattopwin(N)' zeros(1,(Nsin-N)/2)];
    disp('FLAT-TOP ABLAK');
elseif WINDOW_FUNCTION==1
    win=[zeros(1,(Nsin-N)/2) hanning(N)' zeros(1,(Nsin-N)/2)];
    disp('HANNING-ABLAK');
else
    win=[zeros(1,(Nsin-N)/2) ones(1,N) zeros(1,(Nsin-N)/2)];
    disp('NEGYSZOGABLAK');
end
disp(sprintf('fs= %.2f Hz\nN = %d\ndf= %.2f Hz\nf = %.2f Hz = %.2f*df',fs,N,df,f,f/df));
t=(0:Nsin-1)/fs;
freq=(-Nsin/2:Nsin/2-1)*fs/Nsin;
x=cos(2*pi*f*t);
x_lowres=cos(2*pi*f*(0:N-1)/fs);

X=abs(fftshift(fft(x)/Nsin));
WIN=abs(fftshift(fft(win)/sum(win)));
XWIN=abs(fftshift(fft(x_lowres.*win(((Nsin-N)/2+1):((Nsin-N)/2+N))/sum(win),Nsin)));
XWIN_LOWRES=abs(fftshift(fft(x_lowres.*win(((Nsin-N)/2+1):((Nsin-N)/2+N))/sum(win))));

subplot(321);
plot(t,x);
axis([(Nsin-N)/2/fs*0.95 1.05*(Nsin+N)/2/fs min(x)-0.5 max(x)+0.5]);
xlabel('t [s]');
title('Jel');
hold off;

subplot(322);
plot(freq,X);
axis([min(freq) max(freq) 0 max(X)*1.2]);
grid on;
xlabel('f [Hz]');
title('Jel spektruma');
hold off;

subplot(323);
plot(t,win);
axis([(Nsin-N)/2/fs*0.95 1.05*(Nsin+N)/2/fs min(win)-0.5 max(win)+0.5]);
xlabel('t [s]');
title('Ablak');
hold off;

subplot(324);
plot(freq,WIN);
axis([min(freq) max(freq) 0 max(WIN)*1.2]);
grid on;
xlabel('f [Hz]');
title('Ablak spektruma');
hold off;

subplot(325);
plot(t,win.*x);
axis([(Nsin-N)/2/fs*0.95 1.05*(Nsin+N)/2/fs min(win.*x)-0.5 max(win.*x)+0.5]);
xlabel('t [s]');
title('Ablakozott jel');
hold off;

subplot(326);
plot(freq,XWIN);
hold on;
stem((-N/2:N/2-1)*fs/N,XWIN_LOWRES,'r');
axis([min(freq) max(freq) 0 max(XWIN)*1.2]);
grid on;
xlabel('f [Hz]');
title('Ablakozott jel spektruma');
hold off;
