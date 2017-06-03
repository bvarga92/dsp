%% ADAPTIV VONALJAVITAS

clear all;
clc;

%% PARAMETEREK
N=2000;                     %mintak szama (szimulacios ido)
D=10;                       %dekorrelalo kesleltetes
M=300;                      %prediktor egyutthatoinak szama
mu=1e-6;                    %batorsagi tenyezo
y=wgn(1,N,2)+...
  4*sin(2*pi*(0:N-1)*0.02); %a bemeno jel

%% ALE ALGORITMUS
W=zeros(1,M);
y_shr=zeros(1,D);
x_shr=zeros(1,M);
y_est=zeros(1,N);
e=zeros(1,N);
for ii=1:N
    y_shr=[y(ii) y_shr(1:end-1)];
    x_shr=[y_shr(end) x_shr(1:end-1)];
    y_est(ii)=W*x_shr';
    e(ii)=y(ii)-y_est(ii);
    W=W+2*mu*e(ii)*x_shr;
end

%% ABRAZOLAS
figure(1);
subplot(311);
plot(y);
title('Bemeno jel');
ylabel('y');
subplot(312);
plot(e);
title('Egyik komponens');
ylabel('e');
subplot(313);
plot(y_est);
title('Masik komponens');
ylabel('y_{est}');
