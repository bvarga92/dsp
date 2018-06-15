clear all;
clc;

%% PARAMETEREK
w1=100; %elso zarosav vege (rad/s)
w2=300; %ateresztosav kezdete (rad/s)
w3=600; %ateresztosav vege (rad/s)
passbandMaxFall=-1; %maximalis ateresztosavi eses (dB)
stopbandMinAtten=-50; %minimalis zarosavi csillapitas (dB)
fs=2000; %mintaveteli frekvencia (Hz)

%% REFERENS ALULATERESZTO SZAMITASA
w0=sqrt(w2*w3); %ateresztosav kozepe
w4=w0^2/w1; %masodik zarosav kezdete
wp=(w3^2-w0^2)/w3/w0; %referens alulatereszto ateresztosav vege
ws=(w4^2-w0^2)/w4/w0; %referens alulatereszto zarosav kezdete
e=sqrt((1/10^(passbandMaxFall/20))^2-1);
d=10^(stopbandMinAtten/20);
n=ceil(max(log(e)/log(wp),log(1/d^2-1)/(2*log(ws)))); %fokszam
if mod(n,2)==0
    p=exp(1j*pi*(1/2+(1:2*n))/n); %polusok paros n eseten
else
    p=exp(1j*pi*(1:2*n)/n); %polusok paratlan n eseten
end
p(real(p)>0)=[];
den=1;
for ii=1:n
    den=conv(den,[1 -p(ii)]);
end
den=real(den); %atviteli fuggveny nevezoje
syms s;
Hs_ref=1/poly2sym(den,s); %atviteli fuggveny szimbolikusan
w=0:0.01:ws*1.25;
H_ref=20*log10(abs(eval(subs(Hs_ref,s,1j*w))));
figure(1);
limits=[min(w) max(w) min(H_ref) max(H_ref)];
plot(w,H_ref,'r');
hold on;
plot([wp wp],[limits(3) limits(4)],'k--');
plot([1 1],[limits(3) limits(4)],'k--');
plot([ws ws],[limits(3) limits(4)],'k--');
plot([limits(1) limits(2)],[stopbandMinAtten stopbandMinAtten],'k--');
hold off;
axis(limits);
xlabel('w [rad/s]');
ylabel('|H_{ref}| [dB]');
title(sprintf('Referens alulatereszto atvitele (n=%d)',n));

%% TRANSZFORMALAS SAVSZUROVE
Hs=subs(Hs_ref,s,(s^2+w0^2)/s/w0); %a kesz atviteli fuggveny szimbolikusan
w=max(1,w1-200):w4+200;
H=20*log10(abs(eval(subs(Hs,s,1j*w))));
figure(2);
limits=[min(w) max(w) max(min(H),-100) max(H)];
plot(w,H,'r');
hold on;
plot([w1 w1],[limits(3) limits(4)],'k--');
plot([w2 w2],[limits(3) limits(4)],'k--');
plot([w3 w3],[limits(3) limits(4)],'k--');
plot([w4 w4],[limits(3) limits(4)],'k--');
plot([limits(1) limits(2)],[stopbandMinAtten stopbandMinAtten],'k--');
hold off;
axis(limits);
xlabel('\omega [rad/s]');
ylabel('|H(\omega)| [dB]');
title('Az elkeszult szuro atvitele');
[N,D]=numden(simplify(Hs));
sys_cont=minreal(tf(sym2poly(N),sym2poly(D)))

%% BILINEARIS TRANSZFORMACIO (ELOTORZITAS NELKUL)
syms z;
Hz=subs(expand(Hs),s,2*fs*(z-1)/(z+1));
[N,D]=numden(simplify(Hz));
sys_disc=minreal(tf(sym2poly(N),sym2poly(D),1/fs))
