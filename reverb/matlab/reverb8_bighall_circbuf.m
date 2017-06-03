%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                     %
% Big hall reverb effekt 48 kHz mintaveteli frekvenciahoz meretezve.  %
%                                                                     %
% Ez az implementacio cirkularis buffert hasznal, a C koddal valo     %
% minel nagyobb merteku kompatibilitas erdekeben keruli a vektoros    %
% muveleteket a szamitasban, valamint nem hasznal for ciklusokat a    % 
% pipeline muveletvegzes segitese erdekeben.                          %
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
ptr=M; %ez minden kesleltetovonal legregebbi elemere mutat
temp=zeros(8,1);
y=zeros(1,N);
tic;
for ii=1:N
    % olvasas a kesleltelovonalak "vegerol", es a szurok lefuttatasa
    temp(1)=p(1)*temp(1)+g(1)*(1-p(1))*dM(1,ptr(1));
    temp(2)=p(2)*temp(2)+g(2)*(1-p(2))*dM(2,ptr(2));
    temp(3)=p(3)*temp(3)+g(3)*(1-p(3))*dM(3,ptr(3));
    temp(4)=p(4)*temp(4)+g(4)*(1-p(4))*dM(4,ptr(4));
    temp(5)=p(5)*temp(5)+g(5)*(1-p(5))*dM(5,ptr(5));
    temp(6)=p(6)*temp(6)+g(6)*(1-p(6))*dM(6,ptr(6));
    temp(7)=p(7)*temp(7)+g(7)*(1-p(7))*dM(7,ptr(7));
    temp(8)=p(8)*temp(8)+g(8)*(1-p(8))*dM(8,ptr(8));
    % iras a kesleltetovonalak "elejere"
    dM(1,ptr(1))=b(1)*u(ii)+A(1,1)*temp(1)+A(1,2)*temp(2)+A(1,3)*temp(3)+A(1,4)*temp(4)+A(1,5)*temp(5)+A(1,6)*temp(6)+A(1,7)*temp(7)+A(1,8)*temp(8);
    dM(2,ptr(2))=b(2)*u(ii)+A(2,1)*temp(1)+A(2,2)*temp(2)+A(2,3)*temp(3)+A(2,4)*temp(4)+A(2,5)*temp(5)+A(2,6)*temp(6)+A(2,7)*temp(7)+A(2,8)*temp(8);
    dM(3,ptr(3))=b(3)*u(ii)+A(3,1)*temp(1)+A(3,2)*temp(2)+A(3,3)*temp(3)+A(3,4)*temp(4)+A(3,5)*temp(5)+A(3,6)*temp(6)+A(3,7)*temp(7)+A(3,8)*temp(8);
    dM(4,ptr(4))=b(4)*u(ii)+A(4,1)*temp(1)+A(4,2)*temp(2)+A(4,3)*temp(3)+A(4,4)*temp(4)+A(4,5)*temp(5)+A(4,6)*temp(6)+A(4,7)*temp(7)+A(4,8)*temp(8);
    dM(5,ptr(5))=b(5)*u(ii)+A(5,1)*temp(1)+A(5,2)*temp(2)+A(5,3)*temp(3)+A(5,4)*temp(4)+A(5,5)*temp(5)+A(5,6)*temp(6)+A(5,7)*temp(7)+A(5,8)*temp(8);
    dM(6,ptr(6))=b(6)*u(ii)+A(6,1)*temp(1)+A(6,2)*temp(2)+A(6,3)*temp(3)+A(6,4)*temp(4)+A(6,5)*temp(5)+A(6,6)*temp(6)+A(6,7)*temp(7)+A(6,8)*temp(8);
    dM(7,ptr(7))=b(7)*u(ii)+A(7,1)*temp(1)+A(7,2)*temp(2)+A(7,3)*temp(3)+A(7,4)*temp(4)+A(7,5)*temp(5)+A(7,6)*temp(6)+A(7,7)*temp(7)+A(7,8)*temp(8);
    dM(8,ptr(8))=b(8)*u(ii)+A(8,1)*temp(1)+A(8,2)*temp(2)+A(8,3)*temp(3)+A(8,4)*temp(4)+A(8,5)*temp(5)+A(8,6)*temp(6)+A(8,7)*temp(7)+A(8,8)*temp(8);
    % a pointerek leptetese
    if (ptr(1)==1); ptr(1)=M(1); else ptr(1)=ptr(1)-1; end
    if (ptr(2)==1); ptr(2)=M(2); else ptr(2)=ptr(2)-1; end
    if (ptr(3)==1); ptr(3)=M(3); else ptr(3)=ptr(3)-1; end
    if (ptr(4)==1); ptr(4)=M(4); else ptr(4)=ptr(4)-1; end
    if (ptr(5)==1); ptr(5)=M(5); else ptr(5)=ptr(5)-1; end
    if (ptr(6)==1); ptr(6)=M(6); else ptr(6)=ptr(6)-1; end
    if (ptr(7)==1); ptr(7)=M(7); else ptr(7)=ptr(7)-1; end
    if (ptr(8)==1); ptr(8)=M(8); else ptr(8)=ptr(8)-1; end
    % a kimenet osszeallitasa
    y(ii)=d*u(ii)+c(1)*temp(1)+c(2)*temp(2)+c(3)*temp(3)+c(4)*temp(4)+c(5)*temp(5)+c(6)*temp(6)+c(7)*temp(7)+c(8)*temp(8);
    y(ii)=WET*y(ii)+(1-WET)*u(ii);
end
toc;

%-------------- MEGSZOLALTATJUK EGYMAS UTAN ---------------%
wavwrite(y/max(abs(y))/1.1,fs,B,sprintf('%s_reverbed_8_bighall.wav',wavName));
maxsig=max([max(abs(u)) max(abs(y))]);
sound(u/maxsig,fs); % ugy van skalazva, hogy egymashoz kepest jo...
sound(y/maxsig,fs); % ...legyen a hangero, de ne legyen tulvezerles
