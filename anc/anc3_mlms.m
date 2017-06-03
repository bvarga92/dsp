%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                           %
% Tobbcsatornas aktiv zajcsokkentes MLMS algoritmussal                      %
%                                                                           %
%   x      : tavoli zajforras (skalar)                                      %
%   P(z)   : elsodleges utak (zajforras --> mikrofon) atviteleinek vektora  %
%   d      : zaj a mikrofonoknal, ezt akarjuk kioltani (vektor)             %
%   S(z)   : masodlagos utak (hangszoro --> mikrofon) atviteleinek matrixa  %
%   S_id(z): S(z) kozelitese, identifikacio eredmenye (atvitelek matrixa)   %
%   y      : a hangszorokon kiadott jelek (vektor)                          %
%   q      : a hangszorok jelei, mire elernek a mikrofonokhoz (vektor)      %
%   e      : hibajelek vektora (ezt eszleljuk az egyes mikrofonokkal)       %
%   W      : LMS algoritmussal adaptalando szuro (FIR szurok vektora)       %
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
K=2 %csatornak szama
P={                               %elsodleges utak (zajforras --> mikrofon)
    [[0 0 1 0.12]; [1 0.3 0   -0.5]],...  %Z --> M1
    [[0 1 0 0   ]; [1 0.4 0.2 -0.5]]      %Z --> M2
   };
S={                               %masodlagos utak (hangszoro --> mikrofon)
     [[1 -0.4164 1.2346]; [1 0    0.6414]],...  %H1 --> M1
     [[1  0      1.2346]; [1 0.27 0.6414]];     %H1 --> M2
     [[1  0      1.2346]; [1 0.27 0.6414]],...  %H2 --> M1
     [[1 -0.4164 1.2346]; [1 0    0.6414]]      %H2 --> M2
   };
N_S=100 %ennyi tapes FIR szurokkent identifikaljuk az S-eket
mu_id=1e-3; %identifikacio batorsagi tenyezoje
T_id=0.4; %identifikacio ideje (s)
N_W=200 %ennyi tapesek az MLMS adaptiv szuroi
mu_mlms=1e-4; %MLMS batorsagi tenyezoje
noiseSource=@(t) sin(2*pi*440*t)+2*sin(2*pi*sqrt(3)*440*t); %zajforras idofuggvenye
T_sim=2; %szimulacio ideje (s)

%% MASODLAGOS UTAK IDENTIFIKACIOJA LMS ALGORITMUSSAL, FEHERZAJ GERJESZTESSEL
t=0:1/fs:T_id;
y=wgn(1,length(t),1);
S_id=cell(K,K); for mic=1:K; for spkr=1:K; S_id{spkr,mic}=zeros(N_S,1); end; end;
for spkr=1:K
    for mic=1:K
        q=filter(S{spkr,mic}(1,:),S{spkr,mic}(2,:),y);
        e=zeros(1,length(t));
        for ii=N_S:length(t)
            Y=y(ii:-1:(ii-N_S+1))';
            e(ii)=q(ii)-S_id{spkr,mic}'*Y;
            S_id{spkr,mic}=S_id{spkr,mic}+2*mu_id*e(ii)*Y;
        end
        figure(1);
        subplot(K^2,1,(spkr-1)*K+mic);
        plot(1000*t,e);
        title(sprintf('S_{%d,%d} identifikacioja',spkr,mic));
        xlabel('t [ms]');
        ylabel('hibajel');
    end
end

%% AZ MLMS ALGORITMUS FUTTATASA
t=0:1/fs:T_sim;
x=noiseSource(t);
d=zeros(K,length(t));
for mic=1:K; d(mic,:)=filter(P{mic}(1,:),P{mic}(2,:),x); end
W=zeros(N_W,K);
y=zeros(K,length(t));
q=zeros(K,length(t));
e=zeros(K,length(t));
r=cell(K,K); for mic=1:K; for spkr=1:K; r{spkr,mic}=zeros(1,length(t)); end; end;
for ii=N_W:length(t)
    %hangszorok jeleneik szamitasa
    for spkr=1:K
        y(spkr,ii)=-x(ii:-1:(ii-N_W+1))*W(:,spkr);
    end
    %mikrofonok jeleinek szamitasa (a valosagban az e-t merjuk)
    for mic=1:K
        for spkr=1:K
            S_num=S{spkr,mic}(1,:);
            S_den=S{spkr,mic}(2,:);
            qq=filter(S_num,S_den,y(spkr,(ii-length(S_num)+1):ii));
            q(mic,ii)=q(mic,ii)+qq(end);
        end
        e(mic,ii)=d(mic,ii)+q(mic,ii);
    end
    %x szurese
    for mic=1:K
        for spkr=1:K
            r{spkr,mic}(ii)=r{spkr,mic}(ii)+x(ii:-1:(ii-N_S+1))*S_id{spkr,mic};
        end
    end
    %adaptacio
    for spkr=1:K
        delta=zeros(N_W,1);
        for mic=1:K
            delta=delta+e(mic,ii)*r{spkr,mic}(ii:-1:(ii-N_W+1))';
        end
        W(:,spkr)=W(:,spkr)+2*mu_mlms*delta;
    end
end
for mic=1:K
    figure(2);
    subplot(K,1,mic);
    plot(1000*t,e(mic,:));
    title(sprintf('MLMS konvergenciaja (%d. mikrofon)',mic));
    xlabel('t [ms]');
    ylabel('hibajel');
end
