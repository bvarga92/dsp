clear all;
clc;

%% parameterek
fs=8e3; % mintaveteli frekvencia [Hz]
c=340; % hangsebesseg [m/s]
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
Delta11=Delta11_opt;
Delta12=Delta12_opt;
Delta21=Delta21_opt;
Delta22=Delta22_opt;
P1=1;
P2=1;
N=1e4; % szimulacio hossza
omega0=pi/6; % referenciajel diszkret korfrekvenciaja (0...pi)
C=1; % referenciajel amplitudoja
L=100; % adaptiv szurok hossza
mu=maxval/L/C^2

x=C*sin(omega0*(0:N-1));
d1=filter(P1,1,x);
d2=filter(P2,1,x);
W1=zeros(1,L);
W2=zeros(1,L);
S11=[zeros(1,Delta11) A11];
S12=[zeros(1,Delta12) A12];
S21=[zeros(1,Delta21) A21];
S22=[zeros(1,Delta22) A22];
xbuf=zeros(1,max([Delta11+1 Delta12+1 Delta21+1 Delta22+1 L]));
r11=zeros(1,L);
r12=zeros(1,L);
r21=zeros(1,L);
r22=zeros(1,L);
y1=zeros(1,max([Delta11+1 Delta12+1]));
y2=zeros(1,max([Delta21+1 Delta22+1]));
e1=zeros(1,N);
e2=zeros(1,N);
for ii=1:N
    xbuf=[x(ii) xbuf(1:end-1)];
    y1=[W1*xbuf(1:L)' y1(1:end-1)];
    y2=[W2*xbuf(1:L)' y2(1:end-1)];
    e1(ii)=d1(ii)-S11*y1(1:Delta11+1)'-S21*y2(1:Delta21+1)';
    e2(ii)=d2(ii)-S12*y1(1:Delta12+1)'-S22*y2(1:Delta22+1)';
    r11=[S11*xbuf(1:Delta11+1)' r11(1:end-1)];
    r12=[S12*xbuf(1:Delta12+1)' r12(1:end-1)];
    r21=[S21*xbuf(1:Delta21+1)' r21(1:end-1)];
    r22=[S22*xbuf(1:Delta22+1)' r22(1:end-1)];
    W1=W1+2*mu*(e1(ii)*r11+e2(ii)*r12);
    W2=W2+2*mu*(e1(ii)*r21+e2(ii)*r22);
end

figure(3);
subplot(211);
plot(e1);
ylabel('e_1[n]');
subplot(212);
plot(e2);
ylabel('e_2[n]');