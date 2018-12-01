clear all;
clc;

%% PARAMETEREK
fs=8000;                    % mintaveteli frekvencia [Hz]
T=0.5;                      % szimulacios ido [s]
N_S=5;                      % masodlagos utakat modellezo FIR szurok hossza
N_W=300;                    % adaptiv szurok hossza
mu=2e-7;                    % batorsagi tenyezo
updatePeriod=[100 100];     % ennyi mintankent kuldjuk at az egyutthatokat, es frissitjuk az adaptiv szuroket
noise=@(t) sin(2*pi*200*t); % zajforras

%% ELSODLEGES UTAK (zajforras --> mikrofonok)
[PB1,PA1]=ellip(6,1,60,0.3);
[PB2,PA2]=ellip(6,1,60,0.5);

%% MASODLAGOS UTAK (Sij: i. hangszoro --> j. mikrofon)
S11=1:N_S;
S12=2*(N_S:-1:1).*(-1).^(1:N_S);
S21=S12;
S22=S11;

%% IDENTIFIKACIO (most tokeletes, a valosagban LMS algoritmus)
S_id11=S11;
S_id12=S12;
S_id21=S21;
S_id22=S22;

%% ZAJCSOKKENTES
t=0:1/fs:T;
x=noise(t);                  % zajforras (referencia)
y1=filter(PB1,PA1,x);        % zaj az 1. node-nal
y2=filter(PB2,PA2,x);        % zaj a  2. node-nal
x_shr=zeros(1,max(N_W,N_S)); % buffer a referenciajelnek
y_hat_shr=zeros(2,N_S);      % buffer a kimeneti jeleknek
r11=zeros(1,N_W);            % x megszurve S11-gyel
r12=zeros(1,N_W);            % x megszurve S12-vel
r21=zeros(1,N_W);            % x megszurve S21-gyel
r22=zeros(1,N_W);            % x megszurve S22-vel
e=zeros(2,length(t));        % hibajelek (a mikrofonok altal hallott zaj)
W=zeros(2,N_W);              % adaptiv szurok
DW=zeros(2,N_W);             % update-ek
DW_1=zeros(2,N_W);           % node1 altal (a sajat hibajele alapjan) elokeszitett update
DW_2=zeros(2,N_W);           % node2 altal (a sajat hibajele alapjan) elokeszitett update
updateCntr=zeros(1,2);       % ebben szamoljuk az update-ig

for ii=1:length(t);
    % a referenciajelet mindket node megkapja
        x_shr=[x(ii) x_shr(1:end-1)];
    % a hangszoron kiadando jel
        % node1
        y_hat=-W(1,:)*x_shr';
        y_hat_shr(1,:)=[y_hat y_hat_shr(1,1:end-1)];
        % node2
        y_hat=-W(2,:)*x_shr';
        y_hat_shr(2,:)=[y_hat y_hat_shr(2,1:end-1)];
    % a mikrofonok altal erzekelt hibajelek (ezt a "fizika csinalja")
        e(1,ii)=y1(ii)+S11*y_hat_shr(1,:)'+S21*y_hat_shr(2,:)';
        e(2,ii)=y2(ii)+S22*y_hat_shr(2,:)'+S12*y_hat_shr(1,:)';
    % update elokeszitese
        % node1
        r11=[S_id11*x_shr(1:N_S)' r11(1:end-1)];
        r21=[S_id21*x_shr(1:N_S)' r21(1:end-1)];
        DW_1(1,:)=DW_1(1,:)+r11*e(1,ii); % node1 -> node1 update
        DW_1(2,:)=DW_1(2,:)+r21*e(1,ii); % node1 -> node2 update
        % node2
        r22=[S_id22*x_shr(1:N_S)' r22(1:end-1)];
        r12=[S_id12*x_shr(1:N_S)' r12(1:end-1)];
        DW_2(2,:)=DW_2(2,:)+r22*e(2,ii); % node2 -> node2 update
        DW_2(1,:)=DW_2(1,:)+r12*e(2,ii); % node2 -> node1 update
    % kommunikacio: node1  <----DW_2(1,:)------  node2
        updateCntr=updateCntr+1;
        if updateCntr(1)>=updatePeriod(1)
            % node1: megkapja node2-tol DW_2(1,:)-et, DW_1(1,:)-et pedig o szamitja ki
            DW(1,:)=mu*DW_1(1,:)+mu*DW_2(1,:);
            W=W+DW;
            % valtozok nullazasa: csak azt a reszt, amit felhasznaltunk, a tobbit meg novelik majd a node-ok
            DW_1(1,:)=zeros(1,N_W);
            DW_2(1,:)=zeros(1,N_W);
            % teljes update matrix nullazasa 
            DW=zeros(2,N_W);
            updateCntr(1)=0;            
        end
    % kommunikacio: node1  -----DW_1(2,:)----->  node2
        if updateCntr(2)>=updatePeriod(2)
            % node2: megkapja node1-tol DW_1(2,:)-et, DW_2(2,:)-et pedig o szamitja ki
            DW(2,:)=mu*DW_1(2,:)+mu*DW_2(2,:);
            W=W+DW;
            % valtozok nullazasa: csak azt a reszt, amit felhasznaltunk, a tobbit meg novelik majd a node-ok
            DW_1(2,:)=zeros(1,N_W);
            DW_2(2,:)=zeros(1,N_W);
            % teljes update matrix nullazasa 
            DW=zeros(2,N_W);
            updateCntr(2)=0;            
        end
end

%% ABRAZOLAS
figure(1);
subplot(211);
plot(t,y1,'b',t,e(1,:),'r');
xlabel('t [s]');
ylabel('y_1, e_1');
subplot(212);
plot(t,y2,'b',t,e(2,:),'r');
xlabel('t [s]');
ylabel('y_2, e_2');