clear all;
clc;

%% Kmax szamitasa
A11=2; Delta11=20;
A12=1; Delta12=21;
A21=1; Delta21=65;
A22=2; Delta22=60;

roots_0 =@(A11,A12,A21,A22,Delta11,Delta12,Delta21,Delta22) roots([(A11*A22*(1/4+(Delta11+Delta22)/2+Delta11*Delta22)-A12*A21*(1/4+(Delta12+Delta21)/2+Delta12*Delta21))*(A11*A22-A12*A21)   -(A11^2*Delta11+A12^2*Delta12+A21^2*Delta21+A22^2*Delta22+(A11^2+A12^2+A21^2+A22^2)/2)   1]);
roots_pi=@(A11,A12,A21,A22,Delta11,Delta12,Delta21,Delta22) roots([((-1)^(Delta11+Delta22)*A11*A22*(1/4+(Delta11+Delta22)/2+Delta11*Delta22)-(-1)^(Delta12+Delta21)*A12*A21*(1/4+(Delta12+Delta21)/2+Delta12*Delta21))*(A11*A22*(-1)^(Delta11+Delta22)-A12*A21*(-1)^(Delta12+Delta21))   -(A11^2*Delta11+A12^2*Delta12+A21^2*Delta21+A22^2*Delta22+(A11^2+A12^2+A21^2+A22^2)/2)   1]);
minpos=@(x) min(x(x>0));

r0=roots_0(A11,A12,A21,A22,Delta11,Delta12,Delta21,Delta22);
rpi=roots_pi(A11,A12,A21,A22,Delta11,Delta12,Delta21,Delta22);

Kmax=min(minpos(r0),minpos(rpi))

%% kiegyensulyozas vizsgalata
a=linspace(-1,1,1000);
Kmax=zeros(size(a));

for ii=1:length(a)
    Delta11_=round(Delta11*(1+a(ii)));
    Delta12_=round(Delta12*(1+a(ii)));
    Delta21_=round(Delta21*(1-a(ii)));
    Delta22_=round(Delta22*(1-a(ii)));
    r0=roots_0(A11,A12,A21,A22,Delta11_,Delta12_,Delta21_,Delta22_);
    rpi=roots_pi(A11,A12,A21,A22,Delta11_,Delta12_,Delta21_,Delta22_);
    Kmax(ii)=min(minpos(r0),minpos(rpi));
end

figure(1);
plot(a,Kmax);
xlabel('a');
ylabel('K_{max}');

%% parametererzekenyseg vizsgalata
A11=2; Delta11=20;
A12=1; Delta12=21;
A21=1; Delta21=65;
A22=2; Delta22=60;
a=linspace(-1,1,1000);

r0=roots_0(A11,A12,A21,A22,Delta11,Delta12,Delta21,Delta22);
rpi=roots_pi(A11,A12,A21,A22,Delta11,Delta12,Delta21,Delta22);
Kmax=min(minpos(r0),minpos(rpi));
sensitivity=zeros(size(a));
figure(2);
for fig=1:8
    A11_=A11;
    A12_=A12;
    A21_=A21;
    A22_=A22;
    Delta11_=Delta11;
    Delta12_=Delta12;
    Delta21_=Delta21;
    Delta22_=Delta22;
    for ii=1:length(a)
        switch fig
            case 1, A11_=A11*(1+a(ii)); xstr='A_{11} relativ megvaltozasa';
            case 2, A12_=A12*(1+a(ii)); xstr='A_{12} relativ megvaltozasa';
            case 3, A21_=A21*(1+a(ii)); xstr='A_{21} relativ megvaltozasa';
            case 4, A22_=A22*(1+a(ii)); xstr='A_{22} relativ megvaltozasa';
            case 5, Delta11_=round(Delta11*(1+a(ii))); xstr='\Delta_{11} relativ megvaltozasa';
            case 6, Delta12_=round(Delta12*(1+a(ii))); xstr='\Delta_{12} relativ megvaltozasa';
            case 7, Delta21_=round(Delta21*(1+a(ii))); xstr='\Delta_{21} relativ megvaltozasa';
            case 8, Delta22_=round(Delta22*(1+a(ii))); xstr='\Delta_{22} relativ megvaltozasa';
        end
        r0=roots_0(A11_,A12_,A21_,A22_,Delta11_,Delta12_,Delta21_,Delta22_);
        rpi=roots_pi(A11_,A12_,A21_,A22_,Delta11_,Delta12_,Delta21_,Delta22_);
        sensitivity(ii)=min(minpos(r0),minpos(rpi))/Kmax-1;
    end
    subplot(2,4,fig);
    plot(a,sensitivity);
    ylabel('K_{max} relativ megvaltozasa');
    xlabel(xstr);
end

%% abszolut parametererzekenyseg vizsgalata
A11=2; Delta11=20;
A12=1; Delta12=21;
A21=1; Delta21=65;
A22=2; Delta22=60;
pts=1000;
dA11=linspace(-0.8,2,pts);
dA12=linspace(-0.8,2,pts);
dA21=linspace(-0.8,2,pts);
dA22=linspace(-0.8,2,pts);
dDelta11=linspace(-20,20,pts);
dDelta12=linspace(-20,20,pts);
dDelta21=linspace(-20,20,pts);
dDelta22=linspace(-20,20,pts);

r0=roots_0(A11,A12,A21,A22,Delta11,Delta12,Delta21,Delta22);
rpi=roots_pi(A11,A12,A21,A22,Delta11,Delta12,Delta21,Delta22);
Kmax=min(minpos(r0),minpos(rpi));
sensitivity=zeros(size(a));
figure(3);
for fig=1:8
    A11_=A11;
    A12_=A12;
    A21_=A21;
    A22_=A22;
    Delta11_=Delta11;
    Delta12_=Delta12;
    Delta21_=Delta21;
    Delta22_=Delta22;
    for ii=1:pts
        switch fig
            case 1, A11_=A11+dA11(ii); xvar=dA11; xstr='A_{11} megvaltozasa';
            case 2, A12_=A12+dA12(ii); xvar=dA12; xstr='A_{12} megvaltozasa';
            case 3, A21_=A21+dA21(ii); xvar=dA21; xstr='A_{21} megvaltozasa';
            case 4, A22_=A22+dA22(ii); xvar=dA22; xstr='A_{22} megvaltozasa';
            case 5, Delta11_=round(Delta11+dDelta11(ii)); xvar=dDelta11; xstr='\Delta_{11} megvaltozasa';
            case 6, Delta12_=round(Delta12+dDelta12(ii)); xvar=dDelta12; xstr='\Delta_{12} megvaltozasa';
            case 7, Delta21_=round(Delta21+dDelta21(ii)); xvar=dDelta21; xstr='\Delta_{21} megvaltozasa';
            case 8, Delta22_=round(Delta22+dDelta22(ii)); xvar=dDelta22; xstr='\Delta_{22} megvaltozasa';
        end
        r0=roots_0(A11_,A12_,A21_,A22_,Delta11_,Delta12_,Delta21_,Delta22_);
        rpi=roots_pi(A11_,A12_,A21_,A22_,Delta11_,Delta12_,Delta21_,Delta22_);
        sensitivity(ii)=min(minpos(r0),minpos(rpi))/Kmax-1;
    end
    subplot(2,4,fig);
    plot(xvar,sensitivity);
    xlim(minmax(xvar));
    ylabel('K_{max} relativ megvaltozasa');
    xlabel(xstr);
end