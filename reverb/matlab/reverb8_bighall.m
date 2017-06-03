%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                     %
% Big hall reverb effekt 48 kHz mintaveteli frekvenciahoz meretezve.  %
%                                                                     %
% Ennek a kodnak az atlathatosag az elonye, de a masik ket nagysag-   %
% renddel gyorsabb!                                                   %
%                                                                     %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
clc;

%------------------------ BEMENET ------------------------%
wavName='input2'; %a bemeneti wav file neve kiterjesztes nelkul
[u fs B]=wavread(wavName);
%u=[1;zeros(120000,1)]; fs=48000; B=16; % impulzusvalaszhoz (WET=1 legyen!)

%---------------------- PARAMETEREK ----------------------%
% kesleltetovonalak hossza (relativ primek!)
M=[1511 1613 1733 1867 1987 2087 2213 2477];
% kesleltetok utani szurok parameterei: Hi(z)=gi*(1-pi)/(1-pi*z^-1)
g=[0.9213020833 0.9159895833 0.9097395833 0.9027604167...
   0.8965104167 0.8913020833 0.8847395833 0.8709895833]; %DC atvitel
p=[0.1214781931 0.1278146669 0.1350534946 0.1428762747...
   0.1496615575 0.1551653254 0.1619147935 0.1754316918]; %polus
% visszacsatolo matrix (Hadamard)
H2=1/sqrt(2)*[1 1;1 -1];
H4=1/sqrt(2)*[H2 H2;H2 -H2];
H8=1/sqrt(2)*[H4 H4;H4 -H4];
A=H8;
% csillapitas a bemenettol a kesleltetovonalak elejeig
b=[1 1 1 1 1 1 1 1];
% csillapitas a kesleltetovonalak vegetol a kimenetig
c=[-1 1 -1 1 -1 1 -1 1];
% a kimenetre kozvetlenul juto bemenet csillapodasa
d=1;
% a zengetett jel ekkora aranyban keveredik hozza a bemenethez (0<=WET=<1)
WET=0.12;

%------------------------ SZAMITAS ------------------------%
gcdcount=0;
for ii=1:7
    for jj=(ii+1):8
        gcdcount=gcdcount+gcd(M(ii),M(jj));
    end
end
if gcdcount~=nchoosek(8,2);
    disp('A kesleltetesek paronkent nem relativ primek!');
end
disp(sprintf('Bemenet: %.1f kHz, %d bit, %.1f s',fs/1000,B,length(u)/fs));
disp(sprintf(['Terjedesi utak (m): %.1f, %.1f, %.1f, %.1f,\n'...
    '                     %.1f, %.1f, %.1f, %.1f'],M*340/fs));
disp(sprintf('Atlag: %.1f m',mean(M)*340/fs));
disp(sprintf('Osszes kesleltetes (modusszam): %d',sum(M)));
N=length(u);
dM=zeros(8,max(M)); % a kesleltetovonalak matrixa (memoriapazarlo!)
temp=zeros(8,1);
y=zeros(1,N);
tic;
for ii=1:N
    temp=p'.*temp+g'.*(1-p').*[dM(1,M(1));dM(2,M(2));dM(3,M(3));dM(4,M(4));
                               dM(5,M(5));dM(6,M(6));dM(7,M(7));dM(8,M(8))];
    dM=[b'*u(ii)+A*temp dM(:,1:end-1)];
    y(ii)=c*temp+d*u(ii);
end
y=WET*y+(1-WET)*u';
toc;

%-------------- MEGSZOLALTATJUK EGYMAS UTAN ---------------%
wavwrite(y/max(abs(y))/1.1,fs,B,sprintf('%s_reverbed_8_bighall.wav',wavName));
maxsig=max([max(abs(u)) max(abs(y))]);
sound(u/maxsig,fs); % ugy van skalazva, hogy egymashoz kepest jo...
sound(y/maxsig,fs); % ...legyen a hangero, de ne legyen tulvezerles
