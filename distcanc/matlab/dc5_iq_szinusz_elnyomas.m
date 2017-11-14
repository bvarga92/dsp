clear all;
clc;

%% PARAMETEREK
fs=10e6;                  %mintaveteli frekvencia a szimulaciohoz
f_rf=110e3;               %bemeno RF jel frekvenciaja
p_rf=rand*2*pi;           %bemeno RF jel kezdofazisa
f_lo=100e3;               %lokaljelek frekvenciaja
T=2e-3;                   %szimulacios ido
alpha=1e-2;               %megfigyelo batorsagi tenyezoje
beta=1e-3;                %szabalyzo batorsagi tenyezoje (a valosagban kompenzalando az identifikalt atvitellel)
[B,A]=ellip(6,1,60,0.01); %ADC alulatereszto szuroje
%[H,W]=freqz(B,A,10000); plot(W/2/pi*fs,20*log10(abs(H)));

%% JELEK GENERALASA
t=0:1/fs:T;
rf_in=sin(2*pi*f_rf*t+p_rf); %RF bemenet
lo_i=cos(2*pi*f_lo*t);       %I lokaljel
lo_q=sin(2*pi*f_lo*t);       %Q lokaljel
in_i=zeros(1,length(t));     %DSP egyik bemeno jele (rekurzivan all elo)
in_q=zeros(1,length(t));     %DSP masik bemeno jele (rekurzivan all elo)

%% A KOMPLEX JEL NULLARA SZABALYZASA (a valosagban ez persze nem 10 MHz mintaveteli frekvenciaval tortenik)
rf_out=size(t);
buf1=zeros(2,length(B));
buf2=zeros(2,length(A)-1);
r=0;
x=0;
p=0;
e=0;
xSave=zeros(size(t));
eSave=zeros(size(t));
for ii=1:length(t)
    rf_out(ii)=real(r)*lo_i(ii)-imag(r)*lo_q(ii);
    %DSP bemeno jeleinek eloallitasa (a valosagban ez "megtortenik", nem kell szamitani)
    buf1=[(rf_in(ii)-rf_out(ii)).*lo_i(ii) buf1(1,1:end-1) ; (rf_in(ii)-rf_out(ii)).*lo_q(ii) buf1(2,1:end-1)];
    in_i(ii)=1/A(1)*(B*buf1(1,:)'-A(2:end)*buf2(1,:)');
    in_q(ii)=1/A(1)*(B*buf1(2,:)'-A(2:end)*buf2(2,:)');
    buf2=[in_i(ii) buf2(1,1:end-1) ; in_q(ii) buf2(2,1:end-1)];
    %megfigyelo es szabalyzo
    in_cpx=in_i(ii)-1j*in_q(ii);
    c=exp(1j*2*pi*(f_rf-f_lo)/fs*(ii-1));
    y=c*x;
    r=c*p;
    xSave(ii)=x;
    eSave(ii)=e;
    e=in_cpx-y;
    x=x+alpha*e*c';
    if(t(ii)>4e-4); p=p+beta.*x; end; %400 us utan kapcsoljuk be a szabalyzot
end

%% ABRAZOLAS
figure(1);
subplot(221);
plot(t,rf_in,t,rf_out);
title('RF bemenet es a beavatkozojel');
legend('RF_{in}','RF_{out}');
xlabel('t');
ylabel('RF_{in}, RF_{out}');
subplot(223);
plot(t,in_i,t,in_q);
title('ADC bemenete');
legend('in_I','in_Q');
xlabel('t');
ylabel('in_I, in_Q');
subplot(122);
plot(t,abs(xSave));
title('Megfigyelt amplitudo');
xlabel('t');
ylabel('x_1');
