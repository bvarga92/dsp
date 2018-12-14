clear all;
clc;

%% parameterek
fs=8e3; % mintaveteli frekvencia [Hz]
c=340;  % hangsebesseg [m/s]
H=[0+0j  ;  0-1j ; 0-2j]; % hangszorok helye [m]
M=[1+0j  ;  1-1j ; 1-5j]; % mikrofonok helye [m]
B=200*4; % egy uzenet merete [byte]
BW=20e4; % savszelessegkorlat [byte/s]  -->  fs/Delta_comm_1 + fs/Delta_comm_2 + fs/Delta_comm_3 = BW/B

%% fizikai parameterek szamitasa
D=abs([H H H]-[M.' ; M.' ; M.']); % D_ij: H_i es M_j kozti tavolsag
Delta_phy=round(fs*D/c) % a fizikai elrendezesbol adodo kesleltetesek
A=1./D; % a fizikai elrendezesbol adodo erositesek

%% parameterter generalasa
dim=30;
% a=round(fs./(BW/B*linspace(0.1, 0.9, dim)));
a=round(fs./(BW/B*logspace(log10(0.1), log10(0.9), dim)));
[Delta_comm_1, Delta_comm_2]=meshgrid(a, a);
Delta_comm_3=round(1./(BW/(B*fs)-1./Delta_comm_1-1./Delta_comm_2));
Delta_comm_max=1/(BW/B/fs);
Delta_comm_3((Delta_comm_3<Delta_comm_max) | (Delta_comm_3>10*Delta_comm_max))=NaN;
figure(1);
surf(Delta_comm_1,Delta_comm_2,Delta_comm_3);
xlabel('\Delta_{comm1}');
ylabel('\Delta_{comm2}');
zlabel('\Delta_{comm3}');
drawnow;

%% szimulacio
doPlots=false; % abrazoljunk-e a szimulacio kozben?
P1=1;
P2=1;
P3=1;
N=15*fs; % szimulacio hossza
f0=200;  % referenciajel frekvenciaja [Hz]
C=1;     % referenciajel amplitudoja
L=100;   % adaptiv szurok hossza
mu=1e-4; % batorsagi tenyezo
x=C*sin(2*pi*(0:N-1)*f0/fs); % zajforras (referencia)
d1=filter(P1,1,x);           % zaj az 1. node-nal
d2=filter(P2,1,x);           % zaj a  2. node-nal
d3=filter(P3,1,x);           % zaj a  3. node-nal
N_S=max(Delta_phy(:))+1;
settling=cell(dim,dim);
tic;
for di=1:dim
    for dj=1:dim
        fprintf('%d/%d (%.0f s).\n',(di-1)*dim+dj,dim^2,toc);
        sendPeriod=[Delta_comm_1(di,dj) Delta_comm_2(di,dj) Delta_comm_3(di,dj)]; % ennyi mintankent kuldjuk at az egyutthatokat, es frissitjuk az adaptiv szuroket
        if any(isnan(sendPeriod))
            settling{di,dj}=[NaN NaN NaN];
            continue;
        end
        for spkr=1:3
            for mic=1:3
                S{spkr,mic}=[zeros(1,Delta_phy(spkr,mic)) A(spkr,mic) zeros(1,N_S-Delta_phy(spkr,mic)-1)];
                r{spkr,mic}=zeros(1,L); % r{i,j}: x megszurve S{i,j}-vel
            end
        end
        x_shr=zeros(1,max(L,N_S)); % buffer a referenciajelnek
        y_shr=zeros(3,N_S);        % buffer a kimeneti jeleknek
        e=zeros(3,N);              % hibajelek (a mikrofonok altal hallott zaj)
        W=zeros(3,L);              % adaptiv szurok
        DW_1=zeros(3,L);           % node1 altal (a sajat hibajele alapjan) elokeszitett update
        DW_2=zeros(3,L);           % node2 altal (a sajat hibajele alapjan) elokeszitett update
        DW_3=zeros(3,L);           % node3 altal (a sajat hibajele alapjan) elokeszitett update
        sendCntr=zeros(1,3);       % ebben szamoljuk az update ot eltelt idot
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
                % node3
                y=-W(3,:)*x_shr(1:L)';
                y_shr(3,:)=[y y_shr(3,1:end-1)];
            % a mikrofonok altal erzekelt hibajelek (ezt a "fizika csinalja")
                e(1,ii)=d1(ii)+S{1,1}*y_shr(1,:)'+S{2,1}*y_shr(2,:)'+S{3,1}*y_shr(3,:)';
                e(2,ii)=d2(ii)+S{1,2}*y_shr(1,:)'+S{2,2}*y_shr(2,:)'+S{3,2}*y_shr(3,:)';
                e(3,ii)=d3(ii)+S{1,3}*y_shr(1,:)'+S{2,3}*y_shr(2,:)'+S{3,3}*y_shr(3,:)';
            % update elokeszitese
                % node1
                r{1,1}=[S{1,1}*x_shr(1:N_S)' r{1,1}(1:end-1)];
                r{2,1}=[S{2,1}*x_shr(1:N_S)' r{2,1}(1:end-1)];
                r{3,1}=[S{3,1}*x_shr(1:N_S)' r{3,1}(1:end-1)];
                DW_1(1,:)=          r{1,1}*e(1,ii); % node1 -> node1 update
                DW_1(2,:)=DW_1(2,:)+r{2,1}*e(1,ii); % node1 -> node2 update
                DW_1(3,:)=DW_1(3,:)+r{3,1}*e(1,ii); % node1 -> node3 update
                W(1,:)=W(1,:)+mu*DW_1(1,:);         % sajat update vegrehajtasa
                % node2
                r{1,2}=[S{1,2}*x_shr(1:N_S)' r{1,2}(1:end-1)];
                r{2,2}=[S{2,2}*x_shr(1:N_S)' r{2,2}(1:end-1)];
                r{3,2}=[S{3,2}*x_shr(1:N_S)' r{3,2}(1:end-1)];
                DW_2(1,:)=DW_2(1,:)+r{1,2}*e(2,ii); % node2 -> node1 update
                DW_2(2,:)=          r{2,2}*e(2,ii); % node2 -> node2 update
                DW_2(3,:)=DW_2(3,:)+r{3,2}*e(2,ii); % node2 -> node3 update
                W(2,:)=W(2,:)+mu*DW_2(2,:);         % sajat update vegrehajtasa
                % node3
                r{1,3}=[S{1,3}*x_shr(1:N_S)' r{1,3}(1:end-1)];
                r{2,3}=[S{2,3}*x_shr(1:N_S)' r{2,3}(1:end-1)];
                r{3,3}=[S{3,3}*x_shr(1:N_S)' r{3,3}(1:end-1)];
                DW_3(1,:)=DW_3(1,:)+r{1,3}*e(3,ii); % node3 -> node1 update
                DW_3(2,:)=DW_3(2,:)+r{2,3}*e(3,ii); % node3 -> node2 update
                DW_3(3,:)=          r{3,3}*e(3,ii); % node3 -> node3 update
                W(3,:)=W(3,:)+mu*DW_3(3,:);         % sajat update vegrehajtasa
            % kommunikacio: node1  ----- DW_1(2,:), DW_1(3,:) ----->  node2, node3
                sendCntr=sendCntr+1;
                if sendCntr(1)>=sendPeriod(1)
                    % node2: megkapja node1-tol DW_1(2,:)-t
                    W(2,:)=W(2,:)+mu*DW_1(2,:);
                    % node3: megkapja node1-tol DW_1(3,:)-t
                    W(3,:)=W(3,:)+mu*DW_1(3,:);
                    % valtozok nullazasa
                    DW_1=zeros(3,L);
                    sendCntr(1)=0;
                end
            % kommunikacio: node2  ----- DW_2(1,:), DW_2(3,:) ----->  node1, node3
                if sendCntr(2)>=sendPeriod(2)
                    % node1: megkapja node2-tol DW_2(1,:)-t
                    W(1,:)=W(1,:)+mu*DW_2(1,:);
                    % node3: megkapja node2-tol DW_2(3,:)-t
                    W(3,:)=W(3,:)+mu*DW_2(3,:);
                    % valtozok nullazasa
                    DW_2=zeros(3,L);
                    sendCntr(2)=0;
                end
            % kommunikacio: node3  ----- DW_3(1,:), DW_3(2,:) ----->  node1, node2
                if sendCntr(3)>=sendPeriod(3)
                    % node1: megkapja node3-tol DW_3(1,:)-t
                    W(1,:)=W(1,:)+mu*DW_3(1,:);
                    % node2: megkapja node3-tol DW_3(2,:)-t
                    W(2,:)=W(2,:)+mu*DW_3(2,:);
                    % valtozok nullazasa
                    DW_3=zeros(3,L);
                    sendCntr(3)=0;
                end
        end
        for ii=1:3
            if any(abs(e(ii,:))>C*1000)
                settling{di,dj}(ii)=Inf;
            else
                settling{di,dj}(ii)=find(abs(e(ii,:))>C*0.1,1,'last');
            end
        end
        if doPlots
            figure(2);
            for ii=1:3
                subplot(3,1,ii);
                plot((0:N-1)/fs,e(ii,:));
                xlabel('t [s]');
                ylabel(sprintf('e_%d',ii));
                hold on;
                plot([1 1]*settling{di,dj}(ii)/fs,ylim,'r');
                hold off;
                drawnow;
            end
        end
    end
end

%% eredmeny mentese es abrazolasa
save('settling.mat','settling');
figure(3);
surf(Delta_comm_1, Delta_comm_2, cellfun(@max,settling)/fs);
xlabel('\Delta_{comm1}');
ylabel('\Delta_{comm2}');
zlabel('T_{s,max} [s]');