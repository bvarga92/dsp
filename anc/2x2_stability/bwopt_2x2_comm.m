clear all;
clc;

%% parameterek
fs=8e3; % mintaveteli frekvencia [Hz]
c=340;  % hangsebesseg [m/s]
H=[0+0j  ;  0-1j]/2; % hangszorok helye [m]
M=[1+0j  ;  1-5j]/2; % mikrofonok helye [m]
B=200*4; % egy uzenet merete [byte]
BW=20e4; % savszelessegkorlat [byte/s]  -->  fs/Delta_comm_1 + fs/Delta_comm_2 + fs/Delta_comm_3 = BW/B

%% fizikai parameterek szamitasa
D=abs([H H]-[M.' ; M.']); % D_ij: H_i es M_j kozti tavolsag
Delta_phy=round(fs*D/c) % a fizikai elrendezesbol adodo kesleltetesek
A=1./D; % a fizikai elrendezesbol adodo erositesek

%% parameterter generalasa
dim=100;
a=linspace(0.1,0.9,dim); % savszelessegelosztasi arany
Delta_comm_1=round(fs./(BW/B*a));
Delta_comm_2=round(fs./(BW/B*(1-a)));
figure(1);
plot(a,Delta_comm_1,a,Delta_comm_2);
grid on;
xlabel('savszelessegkiosztas');
ylabel('\Delta [minta]');
legend({'\Delta_1','\Delta_2'},'Location','Best');
axis([0 1 0 3*Delta_comm_1(round(length(Delta_comm_1)/2))]);
drawnow;

%% szimulacio
doPlots=true; % abrazoljunk-e a szimulacio kozben?
P1=1;
P2=1;
N=2*fs;   % szimulacio hossza
f0=200;   % referenciajel frekvenciaja [Hz]
C=1;      % referenciajel amplitudoja
L=100;    % adaptiv szurok hossza
mu=2e-5;  % batorsagi tenyezo
x=C*sin(2*pi*(0:N-1)*f0/fs); % zajforras (referencia)
d1=filter(P1,1,x);           % zaj az 1. node-nal
d2=filter(P2,1,x);           % zaj a  2. node-nal
N_S=max(Delta_phy(:))+1;
settling=zeros(dim,2);
tic;
for di=1:dim
    fprintf('%d/%d (%.0f s).\n',di,dim,toc);
    sendPeriod=[Delta_comm_1(di) Delta_comm_2(di)]; % ennyi mintankent kuldjuk at az egyutthatokat, es frissitjuk az adaptiv szuroket
    if any(isnan(sendPeriod))
        settling(di,:)=[NaN NaN];
        continue;
    end
    for spkr=1:2
        for mic=1:2
            S{spkr,mic}=[zeros(1,Delta_phy(spkr,mic)) A(spkr,mic) zeros(1,N_S-Delta_phy(spkr,mic)-1)];
            r{spkr,mic}=zeros(1,L); % r{i,j}: x megszurve S{i,j}-vel
        end
    end
    x_shr=zeros(1,max(L,N_S)); % buffer a referenciajelnek
    y_shr=zeros(2,N_S);        % buffer a kimeneti jeleknek
    e=zeros(2,N);              % hibajelek (a mikrofonok altal hallott zaj)
    W=zeros(2,L);              % adaptiv szurok
    DW_1=zeros(2,L);           % node1 altal (a sajat hibajele alapjan) elokeszitett update
    DW_2=zeros(2,L);           % node2 altal (a sajat hibajele alapjan) elokeszitett update
    sendCntr=zeros(1,2);       % ebben szamoljuk az update ot eltelt idot
    for ii=1:N;
        % a referenciajelet az osszes node megkapja
            x_shr=[x(ii) x_shr(1:end-1)];
        % a hangszoron kiadando jel
            % node1
            y=-W(1,:)*x_shr(1:L)';
            y_shr(1,:)=[y y_shr(1,1:end-1)];
            % node2
            y=-W(2,:)*x_shr(1:L)';
            y_shr(2,:)=[y y_shr(2,1:end-1)];
        % a mikrofonok altal erzekelt hibajelek (ezt a "fizika csinalja")
            e(1,ii)=d1(ii)+S{1,1}*y_shr(1,:)'+S{2,1}*y_shr(2,:)';
            e(2,ii)=d2(ii)+S{1,2}*y_shr(1,:)'+S{2,2}*y_shr(2,:)';
        % update elokeszitese
            % node1
            r{1,1}=[S{1,1}*x_shr(1:N_S)' r{1,1}(1:end-1)];
            r{2,1}=[S{2,1}*x_shr(1:N_S)' r{2,1}(1:end-1)];
            DW_1(1,:)=DW_1(1,:)+r{1,1}*e(1,ii); % node1 -> node1 update
            DW_1(2,:)=DW_1(2,:)+r{2,1}*e(1,ii); % node1 -> node2 update
            % node2
            r{1,2}=[S{1,2}*x_shr(1:N_S)' r{1,2}(1:end-1)];
            r{2,2}=[S{2,2}*x_shr(1:N_S)' r{2,2}(1:end-1)];
            DW_2(1,:)=DW_2(1,:)+r{1,2}*e(2,ii); % node2 -> node1 update
            DW_2(2,:)=DW_2(2,:)+r{2,2}*e(2,ii); % node2 -> node2 update
        % kommunikacio: node1  ----- DW_1(2,:) ----->  node2
            sendCntr=sendCntr+1;
            if sendCntr(1)>=sendPeriod(1)
                % node2: megkapja node1-tol DW_1(2,:)-t
                W(2,:)=W(2,:)+mu*DW_1(2,:)+mu*DW_2(2,:);
                % valtozok nullazasa
                DW_1(2,:)=zeros(1,L);
                DW_2(2,:)=zeros(1,L);
                sendCntr(1)=0;
            end
        % kommunikacio: node2  ----- DW_2(1,:) ----->  node1
            if sendCntr(2)>=sendPeriod(2)
                % node1: megkapja node2-tol DW_2(1,:)-t
                W(1,:)=W(1,:)+mu*DW_2(1,:)+mu*DW_1(1,:);
                % valtozok nullazasa
                DW_2(1,:)=zeros(1,L);
                DW_1(1,:)=zeros(1,L);
                sendCntr(2)=0;
            end
    end
    for ii=1:2
        if any(abs(e(ii,:))>C*1000)
            settling(di,ii)=Inf;
        else
            settling(di,ii)=find(abs(e(ii,:))>C*0.1,1,'last');
        end
    end
    if doPlots
        figure(2);
        for ii=1:2
            subplot(2,1,ii);
            plot((0:N-1)/fs,e(ii,:));
            xlabel('t [s]');
            ylabel(sprintf('e_%d',ii));
            hold on;
            plot([1 1]*settling(di,ii)/fs,ylim,'r');
            hold off;
            drawnow;
        end
    end
end

%% eredmeny mentese es abrazolasa
save('settling.mat','settling');
figure(3);
plot(a, max(settling,[],2)/fs, a, mean(settling,2)/fs);
grid on;
xlabel('savszelessegkiosztas');
ylabel('T_s');
legend('max','atlag','Location','Best');
xlim([0 1]);