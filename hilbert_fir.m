clear all;
clc;

%% parameterek
fs=48e3;
t=0:1/fs:0.1;
x=(1+0.3*sin(2*pi*100*t)).*sin(2*pi*5000*t);
L=100;
compensateDelay=true;

%% Hilbert-szuro
h_hilb=firpm(L,[0.1 0.9],[1 1],'Hilbert');
[H_hilb,w]=freqz(h_hilb,1,8192);
figure(1);
subplot(211);
plot(w*fs/2/pi/1000,20*log10(abs(H_hilb)));
hold on;
plot([0.1 0.1]*fs/2000,ylim,'r');
plot([0.9 0.9]*fs/2000,ylim,'r');
hold off;
xlim([0 fs/2000]);
xlabel('f [kHz]');
ylabel('|H_{hilb}(f)| [dB]');
subplot(212);
plot(w*fs/2/pi/1000,unwrap(angle(H_hilb))/pi*180);
hold on;
plot([0.1 0.1]*fs/2000,ylim,'r');
plot([0.9 0.9]*fs/2000,ylim,'r');
hold off;
xlim([0 fs/2000]);
xlabel('f [kHz]');
ylabel('\phi_{hilb}(f) [fok]');

%% savszuro a kesleltetes kompenzalasara
h_bp=firpm(L,[0 0.05 0.1 0.9 0.95 1],[0 0 1 1 0 0]);
[H_bp,w]=freqz(h_bp,1,8192);
figure(2);
subplot(211);
plot(w*fs/2/pi/1000,20*log10(abs(H_bp)));
hold on;
plot([0.1 0.1]*fs/2000,ylim,'r');
plot([0.9 0.9]*fs/2000,ylim,'r');
hold off;
xlim([0 fs/2000]);
xlabel('f [kHz]');
ylabel('|H_{bp}(f)| [dB]');
subplot(212);
plot(w*fs/2/pi/1000,unwrap(angle(H_bp))/pi*180);
hold on;
plot([0.1 0.1]*fs/2000,ylim,'r');
plot([0.9 0.9]*fs/2000,ylim,'r');
hold off;
xlim([0 fs/2000]);
xlabel('f [kHz]');
ylabel('\phi_{bp}(f) [fok]');

%% a ket jel kozotti faziskulonbseg tenyleg 90 fok lesz?
figure(3);
plot(w*fs/2/pi/1000,180/pi*wrapToPi(unwrap(angle(H_hilb))-unwrap(angle(H_bp))));
hold on;
plot([0.1 0.1]*fs/2000,ylim,'r');
plot([0.9 0.9]*fs/2000,ylim,'r');
hold off;
xlim([0 fs/2000]);
xlabel('f [kHz]');
ylabel('\phi_{hilb}(f) - \phi_{bp}(f) [fok]');

%% burkologorbe eloallitasa
y_hilb=filter(h_hilb,1,x);
y_bp=filter(h_bp,1,x);
if compensateDelay
    D=floor((L-1)/2);
    y_hilb=[y_hilb(D+1:end) ones(1,D)*y_hilb(end)];
    y_bp=[y_bp(D+1:end) ones(1,D)*y_bp(end)];
end
figure(4);
plot(t,x,'b',t,sqrt(y_hilb.^2+y_bp.^2),'r','Linewidth',2);
xlim([t(1) t(end)]);
xlabel('t [s]');
title('Burkolodetektalas');