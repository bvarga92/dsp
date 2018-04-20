clear all;
clc;

x=@(t) 0.97*sin(2*pi*100*t); % a bemeneti jel idofuggvenye
fs=1e6; % a delta-sigma modulator mintaveteli frekvenciaja
decimationFactor=100; % a kimeneti jel mintaveteli frekvenciaja ennyiszer kisebb
N=1e5; % ennyi mintat szimulalunk

t=(0:N-1)/fs;
y=zeros(size(t));
integrator1=0;
integrator2=0;
for ii=2:N
    integrator1=integrator1+x(t(ii))-y(ii-1);
    integrator2=integrator2+integrator1-y(ii-1);
    y(ii)=(integrator2>0)-(integrator2<=0);
end
y_dec=decimate(y,decimationFactor);
tq=(0:length(y_dec)-1)/fs*decimationFactor;

figure(1);
plot(t,y,'g');
hold on;
plot(t,x(t),'b','LineWidth',2);
plot(tq,y_dec,'r');
hold off;
xlim([t(1) t(end)]);
xlabel('t');
legend('Bemeneti jel','\Delta\Sigma moduator kimenete','Kimeneti jel');
figure(2);
plot((0:N-1)*fs/N,abs(fft(x(t)-y))/N);
hold on;
plot([fs/decimationFactor fs/decimationFactor],ylim,'r');
hold off;
xlim([0 fs/2]);
xlabel('f');
ylabel('|N_q(f)|');
title('A kvantalasi zaj spektruma (piros: kimenet mintaveteli frekv.)');
figure(3);
plot(tq,y_dec-x(tq+decimationFactor/fs));
xlim([tq(1) tq(end)]);
xlabel('t');
title('Kvantalasi zaj a kimeneten');