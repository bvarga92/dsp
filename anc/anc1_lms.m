%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                           %
% Rendszeridentifikacio LMS algoritmussal                                   %
%                                                                           %
% Az ismeretlen H(z) atvitelt szeretnenk identifikalni az LMS algoritmus    %
% szerint adaptalt W(z) szurovel.                                           %
%                                                                           %
%                                                                           %
%                                  ------   d                               %
%                           ----->| H(z) |----------                        %
%                           |      ------          |                        %
%                           |                      |                        %
%                    x      |      ------   y      v                        %
%                    ------------>| W(z) |------->(+)                       %
%                           |      ------       -  |                        %
%                           |         ^            |                        %
%                           |         |            |e                       %
%                           |         |            |                        %
%                           |      ------          |                        %
%                           ----->| LMS  |<---------                        %
%                                  ------                                   %
%                                                                           %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
clc;

%% PARAMETEREK
fs=16000; %mintaveteli frekvencia
H=tf([0.3 0],[1 0 0.7],1/fs) %identifikalando rendszer
N_FIR=21; %ha W FIR szuro, akkor ennyi egyutthatos
N_NUM=2;  %ha W IIR szuro, akkor a szamlalo ennyi egyutthatos
N_DEN=3;  %ha W IIR szuro, akkor a nevezo ennyi egyutthatos
mu_fir=1e-3; %batorsagi tenyezo (FIR)
mu_iir=1e-3; %batorsagi tenyezo (IIR)

%% GERJESZTES: veletlen fazisu multiszinusz
t=0:1/fs:0.05;
x=zeros(1,length(t));
for f=0:fs/50:fs/2
    x=x+cos(2*pi*f*t+2*pi*rand(1));
end
d=lsim(H,x);

%% MODELLEZES FIR SZUROKENT
W=zeros(N_FIR,1);
e=zeros(1,length(t));
for ii=(N_FIR+1):length(t)
    X=x((ii-1):-1:(ii-N_FIR))';
    y=W'*X;
    e(ii)=d(ii)-y;
    W=W+2*mu_fir*e(ii)*X;
end
Wfir=W %FIR egyutthatok
h=impulse(H,N_FIR/fs) %valodi impulzusvalasz
figure(1)
subplot(211);
plot(1000*t,e);
title('Konvergencia (FIR)');
xlabel('t [ms]');
ylabel('e');

%% MODELLEZES IIR SZUROKENT
W=zeros(N_NUM+N_DEN-1,1);
e=zeros(1,length(t));
for ii=(N_DEN+1):length(t)
    X=[x((ii-1):-1:(ii-N_NUM)) d((ii-1):-1:(ii-N_DEN+1))']';
    y=W'*X;
    e(ii)=d(ii)-y;
    W=W+2*mu_iir*e(ii)*X;
end
Wnum=W(1:N_NUM) %szamlalo
Wden=[1; -W((N_NUM+1):(N_NUM+N_DEN-1))] %nevezo
subplot(212);
plot(1000*t,e);
title('Konvergencia (IIR)');
xlabel('t [ms]');
ylabel('e');
