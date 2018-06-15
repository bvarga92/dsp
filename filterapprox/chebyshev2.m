clear all;
clc;

%% parameterek
ord=7; % fokszam
ws=10;  % zarosav kezdete
As=60; % zarosavi csillapitas decibelben

%% atviteli karakterisztika eloallitasa
epsilon=sqrt(1/(10^(0.1*As)-1));
p=[];
for ii=0:ord-1
    p(end+1)=1/(1j*cos(1/ord*acos(1j/epsilon)+ii*pi/ord));
    p(end+1)=1/(1j*cos(1/ord*acos(-1j/epsilon)+ii*pi/ord));
end
p=p(real(p)<0);
z=[];
for ii=1:floor(ord/2)
    z(end+1)=1j/cos(pi/(2*ord)*(2*ii-1));
    z(end+1)=-1j/cos(pi/(2*ord)*(2*ii-1));
end
den=1;
for ii=1:length(p)
    den=conv(den,[1 -p(ii)]);
end
den=real(den);
num=1;
for ii=1:length(z)
    num=conv(num,[1 -z(ii)]);
end
num=real(num);
syms s;
Hs=collect(real(prod(p))/real(prod(z))*poly2sym(num,s/ws)/poly2sym(den,s/ws))
w=linspace(0,ws*5,5000);
H=eval(subs(Hs,s,1j*w));

%% abrazolas
w3dB=ws/cosh(1/ord*acosh(1/epsilon));
figure(1);
plot(w,20*log10(abs(H)));
hold on;
plot(w,-3*ones(size(w)),'r--');
plot([w3dB w3dB],ylim,'r--');
hold off;
grid on;
xlabel('\omega');
ylabel('|H(\omega)| [dB]');
figure(2);
plot(p,'rx');
hold on;
plot(z,'bo');
hold off;
grid on;
axis equal;
title('Polus-zerus terkep');
legend('polusok','zerusok');