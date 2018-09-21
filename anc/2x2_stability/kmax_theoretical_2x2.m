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