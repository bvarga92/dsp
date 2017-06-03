%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                           %
% Aktiv zajcsokkentes FXLMS algoritmussal                                   %
%                                                                           %
%   x      : tavoli zajforras                                               %
%   P(z)   : elsodleges ut atvitele (zajforras --> mikrofon)                %
%   d      : zaj a mikrofonnal, ezt akarjuk kioltani                        %
%   S(z)   : masodlagos ut atvitele (hangszoro --> mikrofon)                %
%   S_id(z): S(z) kozelitese, identifikacio eredmenye                       %
%   y      : a hangszoron kiadott jel                                       %
%   q      : a hangszoro jele, mire eler a mikrofonhoz                      %
%   e      : hibajel (ezt eszleljuk a mikrofonnal)                          %
%   W      : LMS algoritmussal adaptalando szuro                            %
%                                                                           %
%                                                                           %
%                                              ------  d                    %
%             ------------------------------->| P(z) |-------               %
%             |                                ------       |               %
%             |                                             |               %
%        x    |                   ------  y    ------  q    v               %
%        ----------------------->| W(z) |---->| S(z) |---->(+)              %
%             |                   ------       ------       |               %
%             |                      ^                      |               %
%             |                      |                      |e              %
%             |                      |                      |               %
%             |    ---------   r   ------                   |               %
%             ----| S_id(z) |---->| LMS  |<------------------               %
%                  ---------       ------                                   %
%                                                                           %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
clc;

%% PARAMETEREK
fs=8000 %mintaveteli frekvencia
P_num=[1 0.12]; P_den=[1 0.3 0 -0.5]; %elsodleges ut (zajforras --> mikrofon)
S_num=[1 -0.4164 1.2346]; S_den=[1 0.6627 0.6414]; %masodlagos ut (hangszoro --> mikrofon)
N_S=100 %ennyi tapes FIR szurokent identifikaljuk S-t
mu_id=1e-3; %identifikacio batorsagi tenyezoje
T_id=0.4; %identifikacio ideje (s)
N_W=200 %ennyi tapes az FXLMS adaptiv szuroje
mu_fxlms=1e-4; %FXLMS batorsagi tenyezoje
noiseSource=@(t) sin(2*pi*440*t)+2*sin(2*pi*sqrt(3)*440*t); %zajforras idofuggvenye
T_sim=0.2; %szimulacio ideje (s)

%% MASODLAGOS UT IDENTIFIKACIOJA LMS ALGORITMUSSAL, FEHERZAJ GERJESZTESSEL
t=0:1/fs:T_id;
y=wgn(1,length(t),1);
q=filter(S_num,S_den,y);
S_id=zeros(N_S,1);
e=zeros(1,length(t));
for ii=N_S:length(t)
    Y=y(ii:-1:(ii-N_S+1))';
    e(ii)=q(ii)-S_id'*Y;
    S_id=S_id+2*mu_id*e(ii)*Y;
end
figure(1);
subplot(211);
plot(1000*t,e);
title('Az identifikacio konvergenciaja');
xlabel('t [ms]');
ylabel('hibajel');

%% AZ FXLMS ALGORITMUS FUTTATASA
t=0:1/fs:T_sim;
x=noiseSource(t);
d=filter(P_num,P_den,x);
y=zeros(1,length(t));
e=zeros(1,length(t));
r=zeros(1,length(t));
W=zeros(N_W,1);
for ii=N_W:length(t)
    y(ii)=-x(ii:-1:(ii-N_W+1))*W;
    q=filter(S_num,S_den,y((ii-length(S_num)+1):ii));
    e(ii)=d(ii)+q(end);
    r(ii)=x(ii:-1:(ii-N_S+1))*S_id;
    W=W+2*mu_fxlms*e(ii)*r(ii:-1:(ii-N_W+1))';
end
figure(1);
subplot(212);
plot(1000*t,e);
title('FXLMS konvergenciaja');
xlabel('t [ms]');
ylabel('hibajel');
