clear all;
clc;

A11=1; Delta11=5;
A12=1; Delta12=10;
A21=1; Delta21=20;
A22=1; Delta22=5;
omega0=0.1;
K=logspace(-3,0,100); % itt K=mu*C^2*L

syms z;
S11=A11*z^-Delta11;
S12=A12*z^-Delta12;
S21=A21*z^-Delta21;
S22=A22*z^-Delta22;
Phi11=-omega0*Delta11;
Phi12=-omega0*Delta12;
Phi21=-omega0*Delta21;
Phi22=-omega0*Delta22;

figure(1);
clf;
rectangle('Position',[-1 -1 2 2],'Curvature',[1 1]);
hold on;
axis([-1.5 1.5 -1.5 1.5]);
grid on;
axis square;
found=false;
for ii=1:length(K)
    alpha11=K(ii)*A11*(cos(omega0-Phi11)*z-cos(Phi11))/(z^2-2*cos(omega0)*z+1);
    alpha12=K(ii)*A12*(cos(omega0-Phi12)*z-cos(Phi12))/(z^2-2*cos(omega0)*z+1);
    alpha21=K(ii)*A21*(cos(omega0-Phi21)*z-cos(Phi21))/(z^2-2*cos(omega0)*z+1);
    alpha22=K(ii)*A22*(cos(omega0-Phi22)*z-cos(Phi22))/(z^2-2*cos(omega0)*z+1);
    % rts=solve(1+alpha11*S11+alpha12*S12+alpha21*S21+alpha22*S22+(alpha11*alpha22-alpha12*alpha21)*(S11*S22-S12*S21)==0,z); % ez nagyon lassu
    rts=roots(sym2poly(collect((1+alpha11*S11+alpha12*S12+alpha21*S21+alpha22*S22+(alpha11*alpha22-alpha12*alpha21)*(S11*S22-S12*S21))*(z^2-2*cos(omega0)*z+1)^2*z^max(Delta11+Delta22,Delta12+Delta21))));
    if(~found && any(abs(rts)>1))
        found=true;
        if ii>1
            fprintf('A stabilitas elveszett.\nK=%f meg jo, K=%f mar nem.\n\n',K(ii-1),K(ii));
        else
            fprintf('Mar a legkisebb K-ra sem stabil.\n\n');
        end
    end
    plot(real(rts),imag(rts),'.','color',[ii/length(K) 0 1-ii/length(K)]);
    drawnow;
end
hold off;