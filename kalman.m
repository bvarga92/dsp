clc;
clear all;

%% PARAMETEREK
fs=64000;                %mintaveteli frekvencia
Tsim=0.05;               %szimulacios ido
[A,B,C,D]=butter(5,0.1); %megfigyelt rendszer allapotvaltozos leirasa
Q=0.0001;                %folyamatzaj varianciaja
R=0.1;                   %meresi zaj varianciaja

%% GERJESZTES
t=0:1/fs:Tsim;
u=square(2*pi*200*t); %gerjesztojel
y=lsim(ss(A,B,C,D,1/fs),u+sqrt(Q)*randn(size(u))); %rendszer kimenete
y_meas=y+sqrt(R)*randn(size(y)); %rendszer megfigyelt kimenete

%% KALMAN-SZURO
P=zeros(size(A));
x_hat=zeros(size(A,1),length(u));
for n=2:length(u)
    %PREDIKCIO
    x_hat_pred=A*x_hat(:,n-1)+B*u(n); %x_hat[n|n-1]=A*x_hat[n-1|n-1]+B*u[n]
    P_pred=A*P*A.'+Q; %P[n|n-1]=A*P[n-1|n-1]*A.'+Q
    %KORREKCIO
    e=y_meas(n)-(C*x_hat_pred+D*u(n)); %e[n]=y_meas[n]-(C*x_hat[n|n-1]+D*u[n])
    S=C*P_pred*C.'+R; %S=C*P[n|n-1]*C.'+R
    K=P_pred*C.'/S; %K=P[n|n-1]*C.'/S
    x_hat(:,n)=x_hat_pred+K*e; %x_hat[n|n]=x_hat[n|n-1]+K*e
    P=(eye(size(P))-K*C)*P_pred; %P[n|n]=(I-K*C)*P[n|n-1]
end
y_est=C*x_hat+D*u;

%% ABRAZOLAS
plot(t,y,'k',t,y_meas,'b',t,y_est,'r');
legend('Valos','Mert','Becsult');
xlabel('t [s]');
