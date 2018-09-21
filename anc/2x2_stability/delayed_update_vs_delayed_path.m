clear all;
clc;

%% parameterek
[PB,PA]=ellip(6,1,60,0.3); % elsodleges ut P=PB/PA
A=2; Delta=50;             % masodlagos ut: S(z)=A*z^-Delta
x=wgn(1,10000,1);          % referenciajel
mu=1e-4;                   % batorsagi tenyezo
L=200;                     % adaptiv szuro hossza

%% 1. eset: kesleltetes a masodlagos utban, azonnali adaptacio
d=filter(PB,PA,x); d=[zeros(1,Delta) d(1:end-Delta)]; % zavarjel
r=[zeros(1,Delta) A*x(1:end-Delta)];                  % szurt referenciajel
e=zeros(size(d));                                     % hibajel
W=zeros(1,L);                                         % adaptiv szuro
y_shr=zeros(1,Delta+1);                               % kimeneti jel buffere
for ii=L:length(d)
    y_shr=[x(ii-L+1:ii)*W' y_shr(1:end-1)];
    y=y_shr(end)*A;
    e(ii)=d(ii)-y;
    W=W+2*mu*r(ii-L+1:ii)*e(ii);
end
figure(1);
subplot(211);
plot(e);
xlabel('n');
ylabel('e');
axis([0 length(e)-1 minmax(e)]);
figure(2);
subplot(211);
plot((0:8191)/8192,20*log10(abs(fft(W,8192))));
[H,omega]=freqz(PB/A,PA,8192);
hold on;
plot(omega/pi/2,20*log10(abs(H)),'r');
hold off;
xlim([0 0.5]);
xlabel('f');
legend('|W(z)| [dB]','|P(z)/S(z)| [dB]');

%% 2. eset: kesleltetett adaptacio, a masodlagos ut kesleltetesmentes
d=filter(PB,PA,x);      % zavarjel
r=A*x;                  % szurt referenciajel
e=zeros(size(d));       % hibajel
W=zeros(1,L);           % adaptiv szuro
dW=zeros(Delta+1,L);    % a sulymodositasok buffere
for ii=L:length(d)
    y=A*x(ii-L+1:ii)*W';
    e(ii)=d(ii)-y;
    dW=[2*mu*r(ii-L+1:ii)*e(ii) ; dW(1:end-1,:)];
    W=W+dW(end,:);
end
figure(1);
subplot(212);
plot(e);
xlabel('n');
ylabel('e');
axis([0 length(e)-1 minmax(e)]);
figure(2);
subplot(212);
plot((0:8191)/8192,20*log10(abs(fft(W,8192))));
[H,omega]=freqz(PB/A,PA,8192);
hold on;
plot(omega/pi/2,20*log10(abs(H)),'r');
hold off;
xlim([0 0.5]);
xlabel('f');
legend('|W(z)| [dB]','|P(z)/S(z)| [dB]');