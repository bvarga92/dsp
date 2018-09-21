clear all;
clc;

A11=1; Delta11=5;
A12=1; Delta12=10;
A21=1; Delta21=20;
A22=1; Delta22=5;
P1=1;
P2=1;
N=1e3; % szimulacio hossza
omega0=pi/6; % referenciajel diszkret korfrekvenciaja (0...pi)
C=1; % referenciajel amplitudoja
mu=0.001; % batorsagi tenyezo
L=100; % adaptiv szurok hossza
K=mu*C^2*L

x=C*sin(omega0*(0:N-1));
d1=filter(P1,1,x);
d2=filter(P2,1,x);
W1=zeros(1,L);
W2=zeros(1,L);
S11=[zeros(1,Delta11) A11];
S12=[zeros(1,Delta12) A12];
S21=[zeros(1,Delta21) A21];
S22=[zeros(1,Delta22) A22];
xbuf=zeros(1,max([Delta11+1 Delta12+1 Delta21+1 Delta22+1 L]));
r11=zeros(1,L);
r12=zeros(1,L);
r21=zeros(1,L);
r22=zeros(1,L);
y1=zeros(1,max([Delta11+1 Delta12+1]));
y2=zeros(1,max([Delta21+1 Delta22+1]));
e1=zeros(1,N);
e2=zeros(1,N);
for ii=1:N
    xbuf=[x(ii) xbuf(1:end-1)];
    y1=[W1*xbuf(1:L)' y1(1:end-1)];
    y2=[W2*xbuf(1:L)' y2(1:end-1)];
    e1(ii)=d1(ii)-S11*y1(1:Delta11+1)'-S21*y2(1:Delta21+1)';
    e2(ii)=d2(ii)-S12*y1(1:Delta12+1)'-S22*y2(1:Delta22+1)';
    r11=[S11*xbuf(1:Delta11+1)' r11(1:end-1)];
    r12=[S12*xbuf(1:Delta12+1)' r12(1:end-1)];
    r21=[S21*xbuf(1:Delta21+1)' r21(1:end-1)];
    r22=[S22*xbuf(1:Delta22+1)' r22(1:end-1)];
    W1=W1+2*mu*(e1(ii)*r11+e2(ii)*r12);
    W2=W2+2*mu*(e1(ii)*r21+e2(ii)*r22);
end

figure(1);
subplot(211);
plot(e1);
ylabel('e_1[n]');
subplot(212);
plot(e2);
ylabel('e_2[n]');