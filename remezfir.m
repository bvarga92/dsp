clear all;
clc;

%% PARAMETEREK (a mintaveteli frekvencia 2)
fp=0.3; % ateresztosav vege
fc=0.4; % vagasi frekvencia
fs=0.5; % zarosav kezdete
Kp=1;   % sulyozas az atereszto tartomanyban
Ks=2;   % sulyozas az elnyomasi tartomanyban
r=15;   % referencapontok szama (szuro hossza: 2r-1)
L=1000; % ennyi pontban ertekeljuk ki az interpolacios fuggvenyt
iter=5; % iteraciok szama
%warning('off','MATLAB:nearlySingularMatrix');

%% REMEZ-ALGORITMUS
% indulo frekvenciakeszlet: egyenletesen, az atmeneti savot kihagyva
rp=floor((r+1)*fp/(fp+1-fs));
fm=[linspace(0.01,fp,rp) linspace(fs,1-.01,r+1-rp)];
% iterativ approximacio
for ii=1:iter
    % egyenletrendszer osszeallitasa es megoldasa
    D=(fm<=fc)';
    W=Kp*(fm<=fp)+Ks*(fm>=fs);
    M=[cos(pi*fm'*(0:r-1)) ((-1).^(1:r+1)')./W'];
    c=M\D;
    delta=c(r+1)
    c=c(1:r);
    % az interpolacios fuggveny es a hiba kiertekelese surun
    f=(0:L-1)/L;
    A=c'*cos(pi*(0:r-1)'*f);
    E=(A-(f<=fc)).*(Kp*(f<=fp)+Ks*(f>=fs));
    % valtakozo elojelu lokalis extremumok keresese
    lmax=find(E>[E(1)-1 , E(1:end-1)] & E>[E(2:end) E(end)-1]);
    lmin=find(E<[E(1)+1 , E(1:end-1)] & E<[E(2:end) E(end)+1]);
    peaks=sort([lmax lmin]);
    k=2;
    while k<=length(peaks)
        if sign(E(peaks(k)))==sign(E(peaks(k-1)))
            peaks(k)=[];
        else
            k=k+1;
        end
    end
    if length(peaks)<r+1
        disp('Hiba: nem sikerult legalabb r+1 extremumot talalni.');
        return;
    else
        peaks=peaks(1:r+1);
    end
    fm=f(peaks); %ez az uj frekvenciakeszlet
    % abrazolas
    figure(1);
    subplot(211);
    plot(f,E);
    hold on;
    plot(fm,E(peaks),'ro');
    plot([0 1],[delta delta],'k--');
    plot([0 1],[-delta -delta],'k--');
    hold off;
    xlim([0 1]);
    xlabel('f_{rel}');
    ylabel('E(f)');
    subplot(212);
    plot(f,A);
    hold on;
    plot(f,f<=fc,'k-.');
    hold off;
    xlim([0 1]);
    xlabel('f_{rel}');
    ylabel('A(f)');
    pause;
end

%% EGYUTTHATOKESZLET SZAMITASA, ELLENORZES
N=2*r-1;
h=[c(r:-1:2)'/2 , c(1) , c(2:r)'/2];
figure(2);
[H,w]=freqz(h,1,8192);
plot(w/pi,20*log10(abs(H)));
grid on;
xlim([0 1])
xlabel('f_{rel}');
ylabel('|H(f)| [dB]');
