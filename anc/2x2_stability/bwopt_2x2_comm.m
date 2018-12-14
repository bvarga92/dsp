clear all;
clc;

%% parameterek
fs=8e3; % mintaveteli frekvencia [Hz]
c=340;  % hangsebesseg [m/s]
H=[0+0j  ;  0-1j]; % hangszorok helye [m]
M=[1+0j  ;  1-5j]; % mikrofonok helye [m]
B=200*4; % egy uzenet merete [byte]
BW=20e4; % savszelessegkorlat [byte/s]  -->  fs/Delta_comm_1 + fs/Delta_comm_2 = BW/B

%% savszelessegelosztas optimumanak keresese
D=abs([H H]-[M.' ; M.']); % D_ij: H_i es M_j kozti tavolsag
Delta_phy=round(fs*D/c); % a fizikai elrendezesbol adodo kesleltetesek
A=1./D; % a fizikai elrendezesbol adodo erositesek
a=linspace(0.01,0.99,1e3); % savszelessegelosztasi arany
Delta_comm_1=round(fs./(BW/B*a));
Delta_comm_2=round(fs./(BW/B*(1-a)));

figure(1);
plot(a,Delta_comm_1,a,Delta_comm_2);
grid on;
xlabel('savszelessegkiosztas');
ylabel('\Delta [minta]');
legend({'\Delta_1','\Delta_2'},'Location','Best');
axis([0 1 0 3*Delta_comm_1(round(length(Delta_comm_1)/2))]);

Delta11=Delta_comm_1+Delta_phy(1,1);
Delta21=Delta_comm_1+Delta_phy(2,1);
Delta12=Delta_comm_2+Delta_phy(1,2);
Delta22=Delta_comm_2+Delta_phy(2,2);

figure(2);
subplot(211);
plot(a, Delta11, a, Delta21, a, Delta12, a, Delta22);
hold on;
hold off;
grid on;
xlabel('savszelessegkiosztas');
ylabel('\Delta [minta]');
legend({'\Delta_{11}', '\Delta_{21}', '\Delta_{12}', '\Delta_{22}'},'Location','Best');
axis([0 1 0 10*Delta_comm_1(round(length(a)/2))]);

roots_0 =@(A11,A12,A21,A22,Delta11,Delta12,Delta21,Delta22) roots([(A11*A22*(1/4+(Delta11+Delta22)/2+Delta11*Delta22)-A12*A21*(1/4+(Delta12+Delta21)/2+Delta12*Delta21))*(A11*A22-A12*A21)   -(A11^2*Delta11+A12^2*Delta12+A21^2*Delta21+A22^2*Delta22+(A11^2+A12^2+A21^2+A22^2)/2)   1]);
roots_pi=@(A11,A12,A21,A22,Delta11,Delta12,Delta21,Delta22) roots([((-1)^(Delta11+Delta22)*A11*A22*(1/4+(Delta11+Delta22)/2+Delta11*Delta22)-(-1)^(Delta12+Delta21)*A12*A21*(1/4+(Delta12+Delta21)/2+Delta12*Delta21))*(A11*A22*(-1)^(Delta11+Delta22)-A12*A21*(-1)^(Delta12+Delta21))   -(A11^2*Delta11+A12^2*Delta12+A21^2*Delta21+A22^2*Delta22+(A11^2+A12^2+A21^2+A22^2)/2)   1]);
minpos=@(x) min(x(x>0));
Kmax=zeros(size(a));
for ii=1:length(a)
    r0=roots_0(A(1,1),A(1,2),A(2,1),A(2,2),Delta11(ii),Delta12(ii),Delta21(ii),Delta22(ii));
    rpi=roots_pi(A(1,1),A(1,2),A(2,1),A(2,2),Delta11(ii),Delta12(ii),Delta21(ii),Delta22(ii));
    Kmax(ii)=min(minpos(r0),minpos(rpi));
end
subplot(212);
plot(a,Kmax);
xlabel('savszelessegkiosztas');
ylabel('K_{max}');
grid on;
[maxval,maxind]=max(Kmax);
hold on;
plot(a(maxind),Kmax(maxind),'ro','MarkerFaceColor','r');
hold off;
axis([0 1 0 maxval*1.1]);

Delta11_opt=Delta11(maxind)
Delta12_opt=Delta12(maxind)
Delta21_opt=Delta21(maxind)
Delta22_opt=Delta22(maxind)

%% szimulacio
A11=A(1,1);
A12=A(1,2);
A21=A(2,1);
A22=A(2,2);
Delta11=Delta_phy(1,1);
Delta21=Delta_phy(2,1);
Delta12=Delta_phy(1,2);
Delta22=Delta_phy(2,2);
P1=1;
P2=1;
updatePeriod=[Delta_comm_2(maxind) Delta_comm_1(maxind)]; % ennyi mintankent kuldjuk at az egyutthatokat, es frissitjuk az adaptiv szuroket
N=2e4;       % szimulacio hossza
omega0=pi/6; % referenciajel diszkret korfrekvenciaja (0...pi)
C=1;         % referenciajel amplitudoja
L=300;       % adaptiv szurok hossza
mu=maxval/L/C^2

N_S=max([Delta11 Delta12 Delta21 Delta22])+1;
S11=[zeros(1,Delta11) A11 zeros(1,N_S-Delta11-1)];
S12=[zeros(1,Delta12) A12 zeros(1,N_S-Delta12-1)];
S21=[zeros(1,Delta21) A21 zeros(1,N_S-Delta21-1)];
S22=[zeros(1,Delta22) A22 zeros(1,N_S-Delta22-1)];

x=C*sin(omega0*(0:N-1));   % zajforras (referencia)
d1=filter(P1,1,x);         % zaj az 1. node-nal
d2=filter(P2,1,x);         % zaj a  2. node-nal
x_shr=zeros(1,max(L,N_S)); % buffer a referenciajelnek
y_shr=zeros(2,N_S);        % buffer a kimeneti jeleknek
r11=zeros(1,L);            % x megszurve S11-gyel
r12=zeros(1,L);            % x megszurve S12-vel
r21=zeros(1,L);            % x megszurve S21-gyel
r22=zeros(1,L);            % x megszurve S22-vel
e=zeros(2,N);              % hibajelek (a mikrofonok altal hallott zaj)
W=zeros(2,L);              % adaptiv szurok
DW=zeros(2,L);             % update-ek
DW_1=zeros(2,L);           % node1 altal (a sajat hibajele alapjan) elokeszitett update
DW_2=zeros(2,L);           % node2 altal (a sajat hibajele alapjan) elokeszitett update
updateCntr=zeros(1,2);     % ebben szamoljuk az update ota eltelt idot

for ii=1:N;
    % a referenciajelet mindket node megkapja
        x_shr=[x(ii) x_shr(1:end-1)];
    % a hangszoron kiadando jel
        % node1
        y=-W(1,:)*x_shr';
        y_shr(1,:)=[y y_shr(1,1:end-1)];
        % node2
        y=-W(2,:)*x_shr';
        y_shr(2,:)=[y y_shr(2,1:end-1)];
    % a mikrofonok altal erzekelt hibajelek (ezt a "fizika csinalja")
        e(1,ii)=d1(ii)+S11*y_shr(1,:)'+S21*y_shr(2,:)';
        e(2,ii)=d2(ii)+S22*y_shr(2,:)'+S12*y_shr(1,:)';
    % update elokeszitese
        % node1
        r11=[S11*x_shr(1:N_S)' r11(1:end-1)];
        r21=[S21*x_shr(1:N_S)' r21(1:end-1)];
        DW_1(1,:)=DW_1(1,:)+r11*e(1,ii); % node1 -> node1 update
        DW_1(2,:)=DW_1(2,:)+r21*e(1,ii); % node1 -> node2 update
        % node2
        r22=[S22*x_shr(1:N_S)' r22(1:end-1)];
        r12=[S12*x_shr(1:N_S)' r12(1:end-1)];
        DW_2(2,:)=DW_2(2,:)+r22*e(2,ii); % node2 -> node2 update
        DW_2(1,:)=DW_2(1,:)+r12*e(2,ii); % node2 -> node1 update
    % kommunikacio: node1  <----DW_2(1,:)------  node2
        updateCntr=updateCntr+1;
        if updateCntr(1)>=updatePeriod(1)
            % node1: megkapja node2-tol DW_2(1,:)-et, DW_1(1,:)-et pedig o szamitja ki
            DW(1,:)=mu*DW_1(1,:)+mu*DW_2(1,:);
            W=W+DW;
            % valtozok nullazasa: csak azt a reszt, amit felhasznaltunk, a tobbit meg novelik majd a node-ok
            DW_1(1,:)=zeros(1,L);
            DW_2(1,:)=zeros(1,L);
            % teljes update matrix nullazasa 
            DW=zeros(2,L);
            updateCntr(1)=0;            
        end
    % kommunikacio: node1  -----DW_1(2,:)----->  node2
        if updateCntr(2)>=updatePeriod(2)
            % node2: megkapja node1-tol DW_1(2,:)-et, DW_2(2,:)-et pedig o szamitja ki
            DW(2,:)=mu*DW_1(2,:)+mu*DW_2(2,:);
            W=W+DW;
            % valtozok nullazasa: csak azt a reszt, amit felhasznaltunk, a tobbit meg novelik majd a node-ok
            DW_1(2,:)=zeros(1,L);
            DW_2(2,:)=zeros(1,L);
            % teljes update matrix nullazasa 
            DW=zeros(2,L);
            updateCntr(2)=0;            
        end
end

settling=[find(abs(e(1,:))>C*0.1,1,'last'), find(abs(e(2,:))>C*0.1,1,'last')]

figure(3);
subplot(211);
plot(e(1,:));
ylabel('e_1');
hold on;
plot([settling(1), settling(1)],ylim,'r');
hold off;
subplot(212);
plot(e(2,:));
hold on;
plot([settling(2), settling(2)],ylim,'r');
hold off;
ylabel('e_2');