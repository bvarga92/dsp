clear all;
clc;

A=1; Delta=15;
P=1;
N=1e3; % szimulacio hossza
omega0=pi/6; % referenciajel diszkret korfrekvenciaja (0...pi)
C=1; % referenciajel amplitudoja
mu=0.001; % batorsagi tenyezo
L=100; % adaptiv szuro hossza
K=mu*C^2*L*A^2

x=C*sin(omega0*(0:N-1));
d=filter(P,1,x);
W=zeros(1,L);
S=[zeros(1,Delta) A];
xbuf=zeros(1,max([Delta+1 L]));
r=zeros(1,L);
y=zeros(1,Delta+1);
e=zeros(1,N);
for ii=1:N
    xbuf=[x(ii) xbuf(1:end-1)];
    y=[W*xbuf(1:L)' y(1:end-1)];
    e(ii)=d(ii)-S*y(1:Delta+1)';
    r=[S*xbuf(1:Delta+1)' r(1:end-1)];
    W=W+2*mu*e(ii)*r;
end

figure(1);
plot(e);
ylabel('e[n]');