clc;
clear all;

%% PARAMETEREK
fs=64000;                       %mintaveteli frekvencia
Tsim=0.05;                      %szimulacios ido
A=0; B=1; C=[1;1;1]; D=[0;0;0]; %allapotvaltozos leiras (3 kimenet a 3 meres)
Q=0.001;                        %folyamatzaj varianciaja
R=0.1*eye(3);                   %meresi zaj varianciaja
drift=20;                       %drift
off=1;                          %ofszet

%% GERJESZTES
t=0:1/fs:Tsim;
u=square(2*pi*200*t); %gerjesztojel
y=lsim(ss(A,B,C,D,1/fs),u+sqrt(Q)*randn(size(u))).'; %rendszer kimenete
y_meas(1,:)=y(1,:)+sqrt(R(1))*randn(size(y(1,:))); %zajos meres
y_meas(2,:)=y(2,:)+drift*t; %driftes meres
y_meas(3,:)=y(3,:)+off; %ofszethibas meres

%% KALMAN-SZURO
P=zeros(size(A));
x_hat=zeros(size(A,1),length(u));
for n=2:length(u)
    %PREDIKCIO
    x_hat_pred=A*x_hat(:,n-1)+B*u(n); %x_hat[n|n-1]=A*x_hat[n-1|n-1]+B*u[n]
    P_pred=A*P*A.'+Q; %P[n|n-1]=A*P[n-1|n-1]*A.'+Q
    %KORREKCIO
    e=y_meas(:,n)-(C*x_hat_pred+D*u(n)); %e[n]=y_meas[n]-(C*x_hat[n|n-1]+D*u[n])
    S=C*P_pred*C.'+R; %S=C*P[n|n-1]*C.'+R
    K=P_pred*C.'/S; %K=P[n|n-1]*C.'*inv(S)
    x_hat(:,n)=x_hat_pred+K*e; %x_hat[n|n]=x_hat[n|n-1]+K*e
    P=(eye(size(P))-K*C)*P_pred; %P[n|n]=(I-K*C)*P[n|n-1]
end
y_est=C*x_hat+D*u;

%% ABRAZOLAS
subplot(311);
plot(t,y);
title('Valos');
xlabel('t [s]');
subplot(312);
plot(t,y_meas);
title('Mert');
xlabel('t [s]');
subplot(313);
plot(t,y_est);
title('Becsult');
xlabel('t [s]');
