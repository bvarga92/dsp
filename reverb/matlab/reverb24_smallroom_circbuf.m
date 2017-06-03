%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                     %
% Small room reverb effekt 48 kHz mintaveteli frekvenciahoz meretezve %
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
M=[181 193 199 223 229 239 251 263 271 281 293 311 317 337 349 359 373 379 383 397 409 421 433 439];
% kesleltetok utani szurok parameterei: Hi(z)=gi*(1-pi)/(1-pi*z^-1)
g=[0.9717187500 0.9698437500 0.9689062500 0.9651562500...
   0.9642187500 0.9626562500 0.9607812500 0.9589062500...
   0.9576562500 0.9560937500 0.9542187500 0.9514062500...
   0.9504687500 0.9473437500 0.9454687500 0.9439062500...
   0.9417187500 0.9407812500 0.9401562500 0.9379687500...
   0.9360937500 0.9342187500 0.9323437500 0.9314062500]; %DC atvitel
p=[0.0509460374 0.0539772207 0.0554787382 0.0613934508...
   0.0628498011 0.0652576937 0.0681157697 0.0709402991...
   0.0728050389 0.0751157651 0.0778595344 0.0819170421...
   0.0832543606 0.0876586267 0.0902625494 0.0924108670...
   0.0953861498 0.0966499030 0.0974886655 0.1004010790...
   0.1028691039 0.1053115057 0.1077287694 0.1089281222]; %polus
% visszacsatolo matrix (Householder)
A=eye(24)-ones(24,24)/12;
% csillapitas a bemenettol a kesleltetovonalak elejeig
b=[1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1];
% csillapitas a kesleltetovonalak vegetol a kimenetig
c=[-1 1 -1 1 -1 1 -1 1 -1 1 -1 1 -1 1 -1 1 -1 1 -1 1 -1 1 -1 1];
% a kimenetre kozvetlenul juto bemenet csillapodasa
d=1;
% a zengetett jel ekkora aranyban keveredik hozza a bemenethez (0<=WET=<1)
WET=0.08;

%------------------------ SZAMITAS ------------------------%
gcdcount=0;
for ii=1:23
    for jj=(ii+1):24
        gcdcount=gcdcount+gcd(M(ii),M(jj));
    end
end
if gcdcount~=nchoosek(24,2);
    disp('A kesleltetesek paronkent nem relativ primek!');
end
disp(sprintf('Bemenet: %.1f kHz, %d bit, %.1f s',fs/1000,B,length(u)/fs));
disp(sprintf(['Terjedesi utak (m): %.2f, %.2f, %.2f, %.2f,\n'...
    '                    %.2f, %.2f, %.2f, %.2f,\n'...
    '                    %.2f, %.2f, %.2f, %.2f,\n'...
    '                    %.2f, %.2f, %.2f, %.2f,\n'...
    '                    %.2f, %.2f, %.2f, %.2f,\n' ...
    '                    %.2f, %.2f, %.2f, %.2f'],M*340/fs));
disp(sprintf('Atlag: %.2f m',mean(M)*340/fs));
disp(sprintf('Osszes kesleltetes (modusszam): %d',sum(M)));
N=length(u);
dM=zeros(24,max(M)); % a kesleltetovonalak matrixa (memoriapazarlo!)
ptr=M; %ez minden kesleltetovonal legregebbi elemere mutat
temp=zeros(24,1);
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
    temp(9)=p(9)*temp(9)+g(9)*(1-p(9))*dM(9,ptr(9));
    temp(10)=p(10)*temp(10)+g(10)*(1-p(10))*dM(10,ptr(10));
    temp(11)=p(11)*temp(11)+g(11)*(1-p(11))*dM(11,ptr(11));
    temp(12)=p(12)*temp(12)+g(12)*(1-p(12))*dM(12,ptr(12));
    temp(13)=p(13)*temp(13)+g(13)*(1-p(13))*dM(13,ptr(13));
    temp(14)=p(14)*temp(14)+g(14)*(1-p(14))*dM(14,ptr(14));
    temp(15)=p(15)*temp(15)+g(15)*(1-p(15))*dM(15,ptr(15));
    temp(16)=p(16)*temp(16)+g(16)*(1-p(16))*dM(16,ptr(16));
    temp(17)=p(17)*temp(17)+g(17)*(1-p(17))*dM(17,ptr(17));
    temp(18)=p(18)*temp(18)+g(18)*(1-p(18))*dM(18,ptr(18));
    temp(19)=p(19)*temp(19)+g(19)*(1-p(19))*dM(19,ptr(19));
    temp(20)=p(20)*temp(20)+g(20)*(1-p(20))*dM(20,ptr(20));
    temp(21)=p(21)*temp(21)+g(21)*(1-p(21))*dM(21,ptr(21));
    temp(22)=p(22)*temp(22)+g(22)*(1-p(22))*dM(22,ptr(22));
    temp(23)=p(23)*temp(23)+g(23)*(1-p(23))*dM(23,ptr(23));
    temp(24)=p(24)*temp(24)+g(24)*(1-p(24))*dM(24,ptr(24));
    % iras a kesleltetovonalak "elejere"
    dM(1,ptr(1))=b(1)*u(ii)+A(1,1)*temp(1)+A(1,2)*temp(2)+A(1,3)*temp(3)+A(1,4)*temp(4)+A(1,5)*temp(5)+A(1,6)*temp(6)+A(1,7)*temp(7)+A(1,8)*temp(8)+A(1,9)*temp(9)+A(1,10)*temp(10)+A(1,11)*temp(11)+A(1,12)*temp(12)+A(1,13)*temp(13)+A(1,14)*temp(14)+A(1,15)*temp(15)+A(1,16)*temp(16)+A(1,17)*temp(17)+A(1,18)*temp(18)+A(1,19)*temp(19)+A(1,20)*temp(20)+A(1,21)*temp(21)+A(1,22)*temp(22)+A(1,23)*temp(23)+A(1,24)*temp(24);
    dM(2,ptr(2))=b(2)*u(ii)+A(2,1)*temp(1)+A(2,2)*temp(2)+A(2,3)*temp(3)+A(2,4)*temp(4)+A(2,5)*temp(5)+A(2,6)*temp(6)+A(2,7)*temp(7)+A(2,8)*temp(8)+A(2,9)*temp(9)+A(2,10)*temp(10)+A(2,11)*temp(11)+A(2,12)*temp(12)+A(2,13)*temp(13)+A(2,14)*temp(14)+A(2,15)*temp(15)+A(2,16)*temp(16)+A(2,17)*temp(17)+A(2,18)*temp(18)+A(2,19)*temp(19)+A(2,20)*temp(20)+A(2,21)*temp(21)+A(2,22)*temp(22)+A(2,23)*temp(23)+A(2,24)*temp(24);
    dM(3,ptr(3))=b(3)*u(ii)+A(3,1)*temp(1)+A(3,2)*temp(2)+A(3,3)*temp(3)+A(3,4)*temp(4)+A(3,5)*temp(5)+A(3,6)*temp(6)+A(3,7)*temp(7)+A(3,8)*temp(8)+A(3,9)*temp(9)+A(3,10)*temp(10)+A(3,11)*temp(11)+A(3,12)*temp(12)+A(3,13)*temp(13)+A(3,14)*temp(14)+A(3,15)*temp(15)+A(3,16)*temp(16)+A(3,17)*temp(17)+A(3,18)*temp(18)+A(3,19)*temp(19)+A(3,20)*temp(20)+A(3,21)*temp(21)+A(3,22)*temp(22)+A(3,23)*temp(23)+A(3,24)*temp(24);
    dM(4,ptr(4))=b(4)*u(ii)+A(4,1)*temp(1)+A(4,2)*temp(2)+A(4,3)*temp(3)+A(4,4)*temp(4)+A(4,5)*temp(5)+A(4,6)*temp(6)+A(4,7)*temp(7)+A(4,8)*temp(8)+A(4,9)*temp(9)+A(4,10)*temp(10)+A(4,11)*temp(11)+A(4,12)*temp(12)+A(4,13)*temp(13)+A(4,14)*temp(14)+A(4,15)*temp(15)+A(4,16)*temp(16)+A(4,17)*temp(17)+A(4,18)*temp(18)+A(4,19)*temp(19)+A(4,20)*temp(20)+A(4,21)*temp(21)+A(4,22)*temp(22)+A(4,23)*temp(23)+A(4,24)*temp(24);
    dM(5,ptr(5))=b(5)*u(ii)+A(5,1)*temp(1)+A(5,2)*temp(2)+A(5,3)*temp(3)+A(5,4)*temp(4)+A(5,5)*temp(5)+A(5,6)*temp(6)+A(5,7)*temp(7)+A(5,8)*temp(8)+A(5,9)*temp(9)+A(5,10)*temp(10)+A(5,11)*temp(11)+A(5,12)*temp(12)+A(5,13)*temp(13)+A(5,14)*temp(14)+A(5,15)*temp(15)+A(5,16)*temp(16)+A(5,17)*temp(17)+A(5,18)*temp(18)+A(5,19)*temp(19)+A(5,20)*temp(20)+A(5,21)*temp(21)+A(5,22)*temp(22)+A(5,23)*temp(23)+A(5,24)*temp(24);
    dM(6,ptr(6))=b(6)*u(ii)+A(6,1)*temp(1)+A(6,2)*temp(2)+A(6,3)*temp(3)+A(6,4)*temp(4)+A(6,5)*temp(5)+A(6,6)*temp(6)+A(6,7)*temp(7)+A(6,8)*temp(8)+A(6,9)*temp(9)+A(6,10)*temp(10)+A(6,11)*temp(11)+A(6,12)*temp(12)+A(6,13)*temp(13)+A(6,14)*temp(14)+A(6,15)*temp(15)+A(6,16)*temp(16)+A(6,17)*temp(17)+A(6,18)*temp(18)+A(6,19)*temp(19)+A(6,20)*temp(20)+A(6,21)*temp(21)+A(6,22)*temp(22)+A(6,23)*temp(23)+A(6,24)*temp(24);
    dM(7,ptr(7))=b(7)*u(ii)+A(7,1)*temp(1)+A(7,2)*temp(2)+A(7,3)*temp(3)+A(7,4)*temp(4)+A(7,5)*temp(5)+A(7,6)*temp(6)+A(7,7)*temp(7)+A(7,8)*temp(8)+A(7,9)*temp(9)+A(7,10)*temp(10)+A(7,11)*temp(11)+A(7,12)*temp(12)+A(7,13)*temp(13)+A(7,14)*temp(14)+A(7,15)*temp(15)+A(7,16)*temp(16)+A(7,17)*temp(17)+A(7,18)*temp(18)+A(7,19)*temp(19)+A(7,20)*temp(20)+A(7,21)*temp(21)+A(7,22)*temp(22)+A(7,23)*temp(23)+A(7,24)*temp(24);
    dM(8,ptr(8))=b(8)*u(ii)+A(8,1)*temp(1)+A(8,2)*temp(2)+A(8,3)*temp(3)+A(8,4)*temp(4)+A(8,5)*temp(5)+A(8,6)*temp(6)+A(8,7)*temp(7)+A(8,8)*temp(8)+A(8,9)*temp(9)+A(8,10)*temp(10)+A(8,11)*temp(11)+A(8,12)*temp(12)+A(8,13)*temp(13)+A(8,14)*temp(14)+A(8,15)*temp(15)+A(8,16)*temp(16)+A(8,17)*temp(17)+A(8,18)*temp(18)+A(8,19)*temp(19)+A(8,20)*temp(20)+A(8,21)*temp(21)+A(8,22)*temp(22)+A(8,23)*temp(23)+A(8,24)*temp(24);
    dM(9,ptr(9))=b(9)*u(ii)+A(9,1)*temp(1)+A(9,2)*temp(2)+A(9,3)*temp(3)+A(9,4)*temp(4)+A(9,5)*temp(5)+A(9,6)*temp(6)+A(9,7)*temp(7)+A(9,8)*temp(8)+A(9,9)*temp(9)+A(9,10)*temp(10)+A(9,11)*temp(11)+A(9,12)*temp(12)+A(9,13)*temp(13)+A(9,14)*temp(14)+A(9,15)*temp(15)+A(9,16)*temp(16)+A(9,17)*temp(17)+A(9,18)*temp(18)+A(9,19)*temp(19)+A(9,20)*temp(20)+A(9,21)*temp(21)+A(9,22)*temp(22)+A(9,23)*temp(23)+A(9,24)*temp(24);
    dM(10,ptr(10))=b(10)*u(ii)+A(10,1)*temp(1)+A(10,2)*temp(2)+A(10,3)*temp(3)+A(10,4)*temp(4)+A(10,5)*temp(5)+A(10,6)*temp(6)+A(10,7)*temp(7)+A(10,8)*temp(8)+A(10,9)*temp(9)+A(10,10)*temp(10)+A(10,11)*temp(11)+A(10,12)*temp(12)+A(10,13)*temp(13)+A(10,14)*temp(14)+A(10,15)*temp(15)+A(10,16)*temp(16)+A(10,17)*temp(17)+A(10,18)*temp(18)+A(10,19)*temp(19)+A(10,20)*temp(20)+A(10,21)*temp(21)+A(10,22)*temp(22)+A(10,23)*temp(23)+A(10,24)*temp(24);
    dM(11,ptr(11))=b(11)*u(ii)+A(11,1)*temp(1)+A(11,2)*temp(2)+A(11,3)*temp(3)+A(11,4)*temp(4)+A(11,5)*temp(5)+A(11,6)*temp(6)+A(11,7)*temp(7)+A(11,8)*temp(8)+A(11,9)*temp(9)+A(11,10)*temp(10)+A(11,11)*temp(11)+A(11,12)*temp(12)+A(11,13)*temp(13)+A(11,14)*temp(14)+A(11,15)*temp(15)+A(11,16)*temp(16)+A(11,17)*temp(17)+A(11,18)*temp(18)+A(11,19)*temp(19)+A(11,20)*temp(20)+A(11,21)*temp(21)+A(11,22)*temp(22)+A(11,23)*temp(23)+A(11,24)*temp(24);
    dM(12,ptr(12))=b(12)*u(ii)+A(12,1)*temp(1)+A(12,2)*temp(2)+A(12,3)*temp(3)+A(12,4)*temp(4)+A(12,5)*temp(5)+A(12,6)*temp(6)+A(12,7)*temp(7)+A(12,8)*temp(8)+A(12,9)*temp(9)+A(12,10)*temp(10)+A(12,11)*temp(11)+A(12,12)*temp(12)+A(12,13)*temp(13)+A(12,14)*temp(14)+A(12,15)*temp(15)+A(12,16)*temp(16)+A(12,17)*temp(17)+A(12,18)*temp(18)+A(12,19)*temp(19)+A(12,20)*temp(20)+A(12,21)*temp(21)+A(12,22)*temp(22)+A(12,23)*temp(23)+A(12,24)*temp(24);
    dM(13,ptr(13))=b(13)*u(ii)+A(13,1)*temp(1)+A(13,2)*temp(2)+A(13,3)*temp(3)+A(13,4)*temp(4)+A(13,5)*temp(5)+A(13,6)*temp(6)+A(13,7)*temp(7)+A(13,8)*temp(8)+A(13,9)*temp(9)+A(13,10)*temp(10)+A(13,11)*temp(11)+A(13,12)*temp(12)+A(13,13)*temp(13)+A(13,14)*temp(14)+A(13,15)*temp(15)+A(13,16)*temp(16)+A(13,17)*temp(17)+A(13,18)*temp(18)+A(13,19)*temp(19)+A(13,20)*temp(20)+A(13,21)*temp(21)+A(13,22)*temp(22)+A(13,23)*temp(23)+A(13,24)*temp(24);
    dM(14,ptr(14))=b(14)*u(ii)+A(14,1)*temp(1)+A(14,2)*temp(2)+A(14,3)*temp(3)+A(14,4)*temp(4)+A(14,5)*temp(5)+A(14,6)*temp(6)+A(14,7)*temp(7)+A(14,8)*temp(8)+A(14,9)*temp(9)+A(14,10)*temp(10)+A(14,11)*temp(11)+A(14,12)*temp(12)+A(14,13)*temp(13)+A(14,14)*temp(14)+A(14,15)*temp(15)+A(14,16)*temp(16)+A(14,17)*temp(17)+A(14,18)*temp(18)+A(14,19)*temp(19)+A(14,20)*temp(20)+A(14,21)*temp(21)+A(14,22)*temp(22)+A(14,23)*temp(23)+A(14,24)*temp(24);
    dM(15,ptr(15))=b(15)*u(ii)+A(15,1)*temp(1)+A(15,2)*temp(2)+A(15,3)*temp(3)+A(15,4)*temp(4)+A(15,5)*temp(5)+A(15,6)*temp(6)+A(15,7)*temp(7)+A(15,8)*temp(8)+A(15,9)*temp(9)+A(15,10)*temp(10)+A(15,11)*temp(11)+A(15,12)*temp(12)+A(15,13)*temp(13)+A(15,14)*temp(14)+A(15,15)*temp(15)+A(15,16)*temp(16)+A(15,17)*temp(17)+A(15,18)*temp(18)+A(15,19)*temp(19)+A(15,20)*temp(20)+A(15,21)*temp(21)+A(15,22)*temp(22)+A(15,23)*temp(23)+A(15,24)*temp(24);
    dM(16,ptr(16))=b(16)*u(ii)+A(16,1)*temp(1)+A(16,2)*temp(2)+A(16,3)*temp(3)+A(16,4)*temp(4)+A(16,5)*temp(5)+A(16,6)*temp(6)+A(16,7)*temp(7)+A(16,8)*temp(8)+A(16,9)*temp(9)+A(16,10)*temp(10)+A(16,11)*temp(11)+A(16,12)*temp(12)+A(16,13)*temp(13)+A(16,14)*temp(14)+A(16,15)*temp(15)+A(16,16)*temp(16)+A(16,17)*temp(17)+A(16,18)*temp(18)+A(16,19)*temp(19)+A(16,20)*temp(20)+A(16,21)*temp(21)+A(16,22)*temp(22)+A(16,23)*temp(23)+A(16,24)*temp(24);
    dM(17,ptr(17))=b(17)*u(ii)+A(17,1)*temp(1)+A(17,2)*temp(2)+A(17,3)*temp(3)+A(17,4)*temp(4)+A(17,5)*temp(5)+A(17,6)*temp(6)+A(17,7)*temp(7)+A(17,8)*temp(8)+A(17,9)*temp(9)+A(17,10)*temp(10)+A(17,11)*temp(11)+A(17,12)*temp(12)+A(17,13)*temp(13)+A(17,14)*temp(14)+A(17,15)*temp(15)+A(17,16)*temp(16)+A(17,17)*temp(17)+A(17,18)*temp(18)+A(17,19)*temp(19)+A(17,20)*temp(20)+A(17,21)*temp(21)+A(17,22)*temp(22)+A(17,23)*temp(23)+A(17,24)*temp(24);
    dM(18,ptr(18))=b(18)*u(ii)+A(18,1)*temp(1)+A(18,2)*temp(2)+A(18,3)*temp(3)+A(18,4)*temp(4)+A(18,5)*temp(5)+A(18,6)*temp(6)+A(18,7)*temp(7)+A(18,8)*temp(8)+A(18,9)*temp(9)+A(18,10)*temp(10)+A(18,11)*temp(11)+A(18,12)*temp(12)+A(18,13)*temp(13)+A(18,14)*temp(14)+A(18,15)*temp(15)+A(18,16)*temp(16)+A(18,17)*temp(17)+A(18,18)*temp(18)+A(18,19)*temp(19)+A(18,20)*temp(20)+A(18,21)*temp(21)+A(18,22)*temp(22)+A(18,23)*temp(23)+A(18,24)*temp(24);
    dM(19,ptr(19))=b(19)*u(ii)+A(19,1)*temp(1)+A(19,2)*temp(2)+A(19,3)*temp(3)+A(19,4)*temp(4)+A(19,5)*temp(5)+A(19,6)*temp(6)+A(19,7)*temp(7)+A(19,8)*temp(8)+A(19,9)*temp(9)+A(19,10)*temp(10)+A(19,11)*temp(11)+A(19,12)*temp(12)+A(19,13)*temp(13)+A(19,14)*temp(14)+A(19,15)*temp(15)+A(19,16)*temp(16)+A(19,17)*temp(17)+A(19,18)*temp(18)+A(19,19)*temp(19)+A(19,20)*temp(20)+A(19,21)*temp(21)+A(19,22)*temp(22)+A(19,23)*temp(23)+A(19,24)*temp(24);
    dM(20,ptr(20))=b(20)*u(ii)+A(20,1)*temp(1)+A(20,2)*temp(2)+A(20,3)*temp(3)+A(20,4)*temp(4)+A(20,5)*temp(5)+A(20,6)*temp(6)+A(20,7)*temp(7)+A(20,8)*temp(8)+A(20,9)*temp(9)+A(20,10)*temp(10)+A(20,11)*temp(11)+A(20,12)*temp(12)+A(20,13)*temp(13)+A(20,14)*temp(14)+A(20,15)*temp(15)+A(20,16)*temp(16)+A(20,17)*temp(17)+A(20,18)*temp(18)+A(20,19)*temp(19)+A(20,20)*temp(20)+A(20,21)*temp(21)+A(20,22)*temp(22)+A(20,23)*temp(23)+A(20,24)*temp(24);
    dM(21,ptr(21))=b(21)*u(ii)+A(21,1)*temp(1)+A(21,2)*temp(2)+A(21,3)*temp(3)+A(21,4)*temp(4)+A(21,5)*temp(5)+A(21,6)*temp(6)+A(21,7)*temp(7)+A(21,8)*temp(8)+A(21,9)*temp(9)+A(21,10)*temp(10)+A(21,11)*temp(11)+A(21,12)*temp(12)+A(21,13)*temp(13)+A(21,14)*temp(14)+A(21,15)*temp(15)+A(21,16)*temp(16)+A(21,17)*temp(17)+A(21,18)*temp(18)+A(21,19)*temp(19)+A(21,20)*temp(20)+A(21,21)*temp(21)+A(21,22)*temp(22)+A(21,23)*temp(23)+A(21,24)*temp(24);
    dM(22,ptr(22))=b(22)*u(ii)+A(22,1)*temp(1)+A(22,2)*temp(2)+A(22,3)*temp(3)+A(22,4)*temp(4)+A(22,5)*temp(5)+A(22,6)*temp(6)+A(22,7)*temp(7)+A(22,8)*temp(8)+A(22,9)*temp(9)+A(22,10)*temp(10)+A(22,11)*temp(11)+A(22,12)*temp(12)+A(22,13)*temp(13)+A(22,14)*temp(14)+A(22,15)*temp(15)+A(22,16)*temp(16)+A(22,17)*temp(17)+A(22,18)*temp(18)+A(22,19)*temp(19)+A(22,20)*temp(20)+A(22,21)*temp(21)+A(22,22)*temp(22)+A(22,23)*temp(23)+A(22,24)*temp(24);
    dM(23,ptr(23))=b(23)*u(ii)+A(23,1)*temp(1)+A(23,2)*temp(2)+A(23,3)*temp(3)+A(23,4)*temp(4)+A(23,5)*temp(5)+A(23,6)*temp(6)+A(23,7)*temp(7)+A(23,8)*temp(8)+A(23,9)*temp(9)+A(23,10)*temp(10)+A(23,11)*temp(11)+A(23,12)*temp(12)+A(23,13)*temp(13)+A(23,14)*temp(14)+A(23,15)*temp(15)+A(23,16)*temp(16)+A(23,17)*temp(17)+A(23,18)*temp(18)+A(23,19)*temp(19)+A(23,20)*temp(20)+A(23,21)*temp(21)+A(23,22)*temp(22)+A(23,23)*temp(23)+A(23,24)*temp(24);
    dM(24,ptr(24))=b(24)*u(ii)+A(24,1)*temp(1)+A(24,2)*temp(2)+A(24,3)*temp(3)+A(24,4)*temp(4)+A(24,5)*temp(5)+A(24,6)*temp(6)+A(24,7)*temp(7)+A(24,8)*temp(8)+A(24,9)*temp(9)+A(24,10)*temp(10)+A(24,11)*temp(11)+A(24,12)*temp(12)+A(24,13)*temp(13)+A(24,14)*temp(14)+A(24,15)*temp(15)+A(24,16)*temp(16)+A(24,17)*temp(17)+A(24,18)*temp(18)+A(24,19)*temp(19)+A(24,20)*temp(20)+A(24,21)*temp(21)+A(24,22)*temp(22)+A(24,23)*temp(23)+A(24,24)*temp(24);
    % a pointerek leptetese
    if (ptr(1)==1); ptr(1)=M(1); else ptr(1)=ptr(1)-1; end
    if (ptr(2)==1); ptr(2)=M(2); else ptr(2)=ptr(2)-1; end
    if (ptr(3)==1); ptr(3)=M(3); else ptr(3)=ptr(3)-1; end
    if (ptr(4)==1); ptr(4)=M(4); else ptr(4)=ptr(4)-1; end
    if (ptr(5)==1); ptr(5)=M(5); else ptr(5)=ptr(5)-1; end
    if (ptr(6)==1); ptr(6)=M(6); else ptr(6)=ptr(6)-1; end
    if (ptr(7)==1); ptr(7)=M(7); else ptr(7)=ptr(7)-1; end
    if (ptr(8)==1); ptr(8)=M(8); else ptr(8)=ptr(8)-1; end
    if (ptr(9)==1); ptr(9)=M(9); else ptr(9)=ptr(9)-1; end
    if (ptr(10)==1); ptr(10)=M(10); else ptr(10)=ptr(10)-1; end
    if (ptr(11)==1); ptr(11)=M(11); else ptr(11)=ptr(11)-1; end
    if (ptr(12)==1); ptr(12)=M(12); else ptr(12)=ptr(12)-1; end
    if (ptr(13)==1); ptr(13)=M(13); else ptr(13)=ptr(13)-1; end
    if (ptr(14)==1); ptr(14)=M(14); else ptr(14)=ptr(14)-1; end
    if (ptr(15)==1); ptr(15)=M(15); else ptr(15)=ptr(15)-1; end
    if (ptr(16)==1); ptr(16)=M(16); else ptr(16)=ptr(16)-1; end
    if (ptr(17)==1); ptr(17)=M(17); else ptr(17)=ptr(17)-1; end
    if (ptr(18)==1); ptr(18)=M(18); else ptr(18)=ptr(18)-1; end
    if (ptr(19)==1); ptr(19)=M(19); else ptr(19)=ptr(19)-1; end
    if (ptr(20)==1); ptr(20)=M(20); else ptr(20)=ptr(20)-1; end
    if (ptr(21)==1); ptr(21)=M(21); else ptr(21)=ptr(21)-1; end
    if (ptr(22)==1); ptr(22)=M(22); else ptr(22)=ptr(22)-1; end
    if (ptr(23)==1); ptr(23)=M(23); else ptr(23)=ptr(23)-1; end
    if (ptr(24)==1); ptr(24)=M(24); else ptr(24)=ptr(24)-1; end
    % a kimenet osszeallitasa
    y(ii)=d*u(ii)+c(1)*temp(1)+c(2)*temp(2)+c(3)*temp(3)+c(4)*temp(4)+c(5)*temp(5)+c(6)*temp(6)+c(7)*temp(7)+c(8)*temp(8)+c(9)*temp(9)+c(10)*temp(10)+c(11)*temp(11)+c(12)*temp(12)+c(13)*temp(13)+c(14)*temp(14)+c(15)*temp(15)+c(16)*temp(16)+c(17)*temp(17)+c(18)*temp(18)+c(19)*temp(19)+c(20)*temp(20)+c(21)*temp(21)+c(22)*temp(22)+c(23)*temp(23)+c(24)*temp(24);
    y(ii)=WET*y(ii)+(1-WET)*u(ii);
end
toc;

%-------------- MEGSZOLALTATJUK EGYMAS UTAN ---------------%
wavwrite(y/max(abs(y))/1.1,fs,B,sprintf('%s_reverbed_24_smallroom.wav',wavName));
maxsig=max([max(abs(u)) max(abs(y))]);
sound(u/maxsig,fs); % ugy van skalazva, hogy egymashoz kepest jo...
sound(y/maxsig,fs); % ...legyen a hangero, de ne legyen tulvezerles
