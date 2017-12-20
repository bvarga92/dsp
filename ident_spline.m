clear all;
clc;

%% PARAMETEREK
f=@(x) atan(x.^3); % statikus nemlinearitas
xmin=-5;           % ertelmezesi tartomany also hatara
xmax=5;            % ertelmezesi tartomany felso hatara
P=20;              % spline kontrollpontok szama
N=1000;            % szimulacio hossza
mu=0.1;            % LMS algoritmus batorsagi tenyezoje
s=xmin+(xmax-xmin)*rand(1,N); % gerjesztes

%% CATMULL-ROM-SPLINE ILLESZTESE LMS ALGORITMUSSAL
M=[0 1 0 0;-0.5 0 0.5 0;1 -2.5 2 -0.5;-0.5 1.5 -1.5 0.5];
x=linspace(xmin,xmax,P);
q=zeros(P,1); % a kontrollpontok vektora
for ii=1:N
    idx=sum(s(ii)>=x); % az aktualis bemenetet megelozo kontrollpont indexe
    if idx==1 || idx>=P-1; continue; end;
    t=(s(ii)-x(idx))/(x(idx+1)-x(idx)); % az intervallum hanyadreszenel van a bemeneti adat
    u=[1 t t^2 t^3];
    e(ii)=f(s(ii))-u*M*q((idx-1):(idx+2)); % hibajel
    q((idx-1):(idx+2))=q((idx-1):(idx+2))+mu*(u*M)'*e(ii); % frissitjuk a megfelelo 4 kontrollpontot
end

%% ABRAZOLAS
xcont=linspace(xmin,xmax,1000);
figure(1);
subplot(211);
plot(xcont,f(xcont));
xlim([xmin xmax]);
hold on;
plot(x,q,'ro');
hold off;
title('Nemlinearitas + spline kontrollpontok');
xlabel('x');
ylabel('f(x)');
subplot(212)
plot(e);
title('Hibajel');
xlabel('n');
ylabel('e');
