clear all;
clc;

%% parameterek
fs=8e3; % mintaveteli frekvencia [Hz]
c=340; % hangsebesseg [m/s]
H=[0+0j  ;  0-1j]; % hangszorok helye [m]
M=[1+0j  ;  1-5j]; % mikrofonok helye [m]
Bp=32; % ennyi biten abrazoljuk a hibajel egy mintajat [bit]
Bh=128; % csomagfejlec merete [bit]
BW=600*1024; % savszelessegkorlat [bit/s]  -->  1/Delta_comm_1 + 1/Delta_comm_2 = (BW-2*fs*Bp)/(fs*Bh)
maxCommDelay=500; % ennel nagyobb kommunikacios kesleltetest nem engedunk meg [minta]

%% savszelessegelosztas optimumanak keresese az elmelet alapjan
D=abs([H H]-[M.' ; M.']); % D_ij: H_i es M_j kozti tavolsag
Delta_phy=round(fs*D/c); % a fizikai elrendezesbol adodo kesleltetesek
A=1./D; % a fizikai elrendezesbol adodo erositesek
a=linspace(0,1,1e3); % savszelessegelosztasi arany
Delta_comm_1=round((fs*Bh)./(BW-2*fs*Bp)./a);
Delta_comm_2=round((fs*Bh)./(BW-2*fs*Bp)./(1-a));
ind=find(Delta_comm_1>0 & Delta_comm_2>0 & Delta_comm_1<=maxCommDelay & Delta_comm_2<=maxCommDelay);
if isempty(ind)
    error('Ilyen parameterek mellett nem uzemeltetheto a rendszer!');
end
a=a(ind);
Delta_comm_1=Delta_comm_1(ind);
Delta_comm_2=Delta_comm_2(ind);

Delta11=Delta_comm_1+Delta_phy(1,1);
Delta21=Delta_comm_1+Delta_phy(2,1);
Delta12=Delta_comm_2+Delta_phy(1,2);
Delta22=Delta_comm_2+Delta_phy(2,2);

roots_0 =@(A11,A12,A21,A22,Delta11,Delta12,Delta21,Delta22) roots([(A11*A22*(1/4+(Delta11+Delta22)/2+Delta11*Delta22)-A12*A21*(1/4+(Delta12+Delta21)/2+Delta12*Delta21))*(A11*A22-A12*A21)   -(A11^2*Delta11+A12^2*Delta12+A21^2*Delta21+A22^2*Delta22+(A11^2+A12^2+A21^2+A22^2)/2)   1]);
roots_pi=@(A11,A12,A21,A22,Delta11,Delta12,Delta21,Delta22) roots([((-1)^(Delta11+Delta22)*A11*A22*(1/4+(Delta11+Delta22)/2+Delta11*Delta22)-(-1)^(Delta12+Delta21)*A12*A21*(1/4+(Delta12+Delta21)/2+Delta12*Delta21))*(A11*A22*(-1)^(Delta11+Delta22)-A12*A21*(-1)^(Delta12+Delta21))   -(A11^2*Delta11+A12^2*Delta12+A21^2*Delta21+A22^2*Delta22+(A11^2+A12^2+A21^2+A22^2)/2)   1]);
minpos=@(x) min(x(x>0));
Kmax=zeros(size(a));
for ii=1:length(a)
    r0=roots_0(A(1,1),A(1,2),A(2,1),A(2,2),Delta11(ii),Delta12(ii),Delta21(ii),Delta22(ii));
    rpi=roots_pi(A(1,1),A(1,2),A(2,1),A(2,2),Delta11(ii),Delta12(ii),Delta21(ii),Delta22(ii));
    Kmax(ii)=min(minpos(r0),minpos(rpi));
end
[maxval,maxind]=max(Kmax);
Delta11_opt=Delta11(maxind)
Delta12_opt=Delta12(maxind)
Delta21_opt=Delta21(maxind)
Delta22_opt=Delta22(maxind)

figure(1);
plot(a,Delta_comm_1,a,Delta_comm_2);
hold on;
plot(repmat(a(maxind),1,2),[Delta_comm_1(maxind) Delta_comm_2(maxind)],'ro','MarkerFaceColor','r');
hold off;
grid on;
xlabel('savszelessegkiosztas');
ylabel('\Delta [minta]');
legend({'\Delta_1','\Delta_2'},'Location','Best');
axis([0 1 0 max(Delta_comm_1)]);

figure(2);
subplot(211);
plot(a, Delta11, a, Delta21, a, Delta12, a, Delta22);
hold on;
plot(repmat(a(maxind),1,4),[Delta11(maxind) Delta12(maxind) Delta21(maxind) Delta22(maxind)],'ro','MarkerFaceColor','r');
hold off;
grid on;
xlabel('savszelessegkiosztas');
ylabel('\Delta [minta]');
legend({'\Delta_{11}', '\Delta_{21}', '\Delta_{12}', '\Delta_{22}'},'Location','Best');
axis([0 1 0 10*Delta_comm_1(round(length(a)/2))]);

subplot(212);
plot(a,Kmax);
xlabel('savszelessegkiosztas');
ylabel('K_{max}');
hold on;
plot(a(maxind),maxval,'ro','MarkerFaceColor','r');
hold off;
grid on;
axis([0 1 0 maxval*1.1]);
drawnow;

%% szimulacio
P1=1;
P2=1;
N=2e4; % szimulacio hossza
f0=400; % referenciajel frekvenciaja [Hz]
C=1; % referenciajel amplitudoja
L=200; % adaptiv szurok hossza
a_ind=1:1:length(a); % ezekben a pontokban szimulalunk
doPlots=false; % szimulacio kozben rajzoljuk ki a hibajeleket?

if length(a_ind)>1
    a_ind=unique([a_ind maxind]);
end
x=C*sin(2*pi*f0/fs*(0:N-1));
d1=filter(P1,1,x);
d2=filter(P2,1,x);
t=(0:N-1)/fs;
settlingRMS=zeros(length(a_ind),2);
str=[];
tsum=0;
tic;
for indind=1:length(a_ind)
    ind=a_ind(indind);
    mu=Kmax(ind)/C^2/L*2.5; % batorsagi tenyezo
    W1=zeros(1,L);
    W2=zeros(1,L);
    S11=[zeros(1,Delta11(ind)) A(1,1)];
    S12=[zeros(1,Delta12(ind)) A(1,2)];
    S21=[zeros(1,Delta21(ind)) A(2,1)];
    S22=[zeros(1,Delta22(ind)) A(2,2)];
    xbuf=zeros(1,max([Delta11(ind)+1 Delta12(ind)+1 Delta21(ind)+1 Delta22(ind)+1 L]));
    r11=zeros(1,L);
    r12=zeros(1,L);
    r21=zeros(1,L);
    r22=zeros(1,L);
    y1=zeros(1,max([Delta11(ind)+1 Delta12(ind)+1]));
    y2=zeros(1,max([Delta21(ind)+1 Delta22(ind)+1]));
    e1=zeros(1,N);
    e2=zeros(1,N);
    for ii=1:N
        xbuf=[x(ii) xbuf(1:end-1)];
        y1=[W1*xbuf(1:L)' y1(1:end-1)];
        y2=[W2*xbuf(1:L)' y2(1:end-1)];
        e1(ii)=d1(ii)-S11*y1(1:Delta11(ind)+1)'-S21*y2(1:Delta21(ind)+1)';
        e2(ii)=d2(ii)-S12*y1(1:Delta12(ind)+1)'-S22*y2(1:Delta22(ind)+1)';
        r11=[S11*xbuf(1:Delta11(ind)+1)' r11(1:end-1)];
        r12=[S12*xbuf(1:Delta12(ind)+1)' r12(1:end-1)];
        r21=[S21*xbuf(1:Delta21(ind)+1)' r21(1:end-1)];
        r22=[S22*xbuf(1:Delta22(ind)+1)' r22(1:end-1)];
        W1=W1+2*mu*(e1(ii)*r11+e2(ii)*r12);
        W2=W2+2*mu*(e1(ii)*r21+e2(ii)*r22);
    end

    Np=5*round(fs/f0);
    e1RMS=sqrt(filter(ones(1,Np)/Np,1,e1.^2));
    e2RMS=sqrt(filter(ones(1,Np)/Np,1,e2.^2));
    xRMS=sqrt(mean(x.^2));
    settlingRMS(indind,:)=[find(abs(e1RMS)>xRMS*0.1,1,'last')-1, find(abs(e2RMS)>xRMS*0.1,1,'last')-1]/fs;

    if doPlots
        figure(3);
        subplot(211);
        plot(t,e1);
        xlim(minmax(t));
        ylabel('e_1');
        xlabel('t [s]')
        hold on;
        plot([settlingRMS(indind,1), settlingRMS(indind,1)],ylim,'r');
        hold off;
        subplot(212);
        plot(t,e2);
        xlim(minmax(t));
        ylabel('e_2');
        xlabel('t [s]')
        hold on;
        plot([settlingRMS(indind,2), settlingRMS(indind,2)],ylim,'r');
        hold off;
        drawnow;
    end
    
    tcur=toc;
    tsum=(ii==1)*tcur+(ii>1)*(0.5*tcur+0.5*tsum);
    trem=tsum*(length(a_ind)-indind);
    fprintf(repmat('\b',1,length(str)));
    str=sprintf('Szimulacio: %d/%d\nHatra van: %.1f min\n',ind,length(a),trem/60);
    fprintf(str);
    tic;
end
fprintf(repmat('\b',1,length(str)));

if length(a_ind)>1
    [~,minind]=min(settlingRMS);
    figure(4);
    plot(a(a_ind),settlingRMS);
    hold on;
    plot([a(maxind) a(maxind)],[settlingRMS(a_ind==maxind,1) settlingRMS(a_ind==maxind,2)],'ro','MarkerFaceColor','r');
    plot(a(a_ind(minind)),[settlingRMS(minind(1),1) settlingRMS(minind(2),2)],'go','MarkerFaceColor','g');
    hold off;
    xlim([0 1]);
    grid on;
    xlabel('savszelessegkiosztas');
    ylabel('T_s [s]');
    legend('mote 1','mote 2','szamitott optimum','szimulalt optimum');
else
    fprintf('Beallas: %.4f es %.4f s.\n\n',settlingRMS(1,1),settlingRMS(1,2));
end