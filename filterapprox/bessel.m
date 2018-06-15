clear all;
clc;

%% parameterek
ord=3; % fokszam
w0=10; % toresponti korfrekvencia

%% az atviteli karakterisztika eloallitasa
den=arrayfun(@(k)prod(k+1:k+ord)/factorial(ord-k)/2^k,0:ord); % ord fokszamu forditott Bessel-polinom egyutthatoi
syms s;
Hs=collect(polyval(den,0)/poly2sym(den,s/w0)) % atviteli fuggveny
w=linspace(0,w0*100,5000);
H=eval(subs(Hs,s,1j*w));

%% abrazolas
figure(1);
semilogx(w,20*log10(abs(H)));
grid on;
xlabel('\omega');
ylabel('|H(\omega)| [dB]');
figure(2);
plot(roots(den),'rx');
grid on;
axis equal;
title('Polusok');