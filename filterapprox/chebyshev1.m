clear all;
clc;

%% parameterek
ord=4;   % fokszam
w0=10;   % toresponti korfrekvencia
delta=1; % ateresztosavi ingadozas decibelben

%% atviteli karakterisztika eloallitasa
epsilon=sqrt(10^(0.1*delta)-1);
p=[];
for ii=0:ord-1
    p(end+1)=1j*cos(1/ord*acos(1j/epsilon)+ii*pi/ord);
    p(end+1)=1j*cos(1/ord*acos(-1j/epsilon)+ii*pi/ord);
end
p=p(real(p)<0); % a szuro polusai
den=1;
for ii=1:ord
    den=conv(den,[1 -p(ii)]);
end
den=real(den); % numerikus okokbol
syms s;
Hs=collect(1/(epsilon*2^(ord-1))*1/poly2sym(den,s/w0)) % atviteli fuggveny
w=linspace(0,w0*5,5000);
H=eval(subs(Hs,s,1j*w));

%% abrazolas
w3dB=w0*cosh(1/ord*acosh(1/epsilon)); % -3 dB-es pont
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
grid on;
axis equal;
title('Polusok');