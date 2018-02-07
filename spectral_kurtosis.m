clear all;
clc;

%% PARAMETEREK
M=900;  % ennyi blokkra bontjuk a jelet a kurtozis szamitasakor
N=1024; % egy blokk ennyi mintat tartalmaz
L=N*M;  % a jel ennyi minta hosszu lesz
signal=@(t) ... % a vizsgalt jel
              (t<2e5).*sin(2*pi*0.1*t)+... % nemstacionarius komponens
              sin(2*pi*0.2*t)+... % stacionarius szinuszos komponens --> -1
              0.25*chirp(t,0.3,t(end),0.33)+... % nemstacionarius komponens
              wgn(1,L,3); % feher Gauss-zaj
p=0.7*exp(-1j*2*pi*0.4); % a rezonator polusa
rez=tf([1 -1/real(p)],[1 -2*real(p) abs(p)^2],1); % ezzel a rezonans rendszerrel is megszurjuk

%% SPEKTRALIS KURTOZIS SZAMITASA
t=(0:L-1);
x=signal(t);
x=lsim(rez,x).';
num=zeros(1,N);
denr=zeros(1,N);
for ii=1:M
    X=fft(x(((ii-1)*N+1):(ii*N)));
    num=num+(M+1)*abs(X).^4;
    denr=denr+abs(X).^2;
end
K=M/(M-1)*(num./(denr.^2)-2);

%% ABRAZOLAS
figure(1);
subplot(311);
plot(t,x);
xlim([t(1) t(end)]);
xlabel('t');
ylabel('x(t)');
subplot(312);
s=psd(x,4096);
f=(0:(length(s)-1))*1/length(s)/2;
plot(f,10*log10(s));
xlim([f(1) f(end)])
xlabel('f');
ylabel('PSD [dB]');
subplot(313);
f=(0:(N-1))*1/N;
plot(f,K);
axis([0 0.5 min(K) max(K)]);
xlabel('f');
ylabel('K(f)');
