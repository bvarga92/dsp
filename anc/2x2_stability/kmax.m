clear all;
clc;

Delta=15;
omega0=linspace(0.001,pi-0.001,500);

Kmax=zeros(size(omega0)); % itt K=mu*C^2*L*A^2
for ii=1:length(omega0)
    K=1;
    step=1;
    while step>1e-5
        Num=[zeros(1,Delta+1)   cos(omega0(ii)*(Delta+1))   -cos(omega0(ii)*Delta)];
        Den=[1   -2*cos(omega0(ii))   1   zeros(1,Delta)];
        if any(abs(roots(Den+K*Num))>=1)
            step=step/2;
            K=K-step;
        else
            step=step/2;
            K=K+step;
        end
    end
    Kmax(ii)=K;
end

figure(1);
plot(omega0,Kmax);
axis([minmax(omega0) minmax(Kmax)]);
xlabel('\omega_0');
ylabel('K_{max}');
title(sprintf('\\Delta=%d',Delta));