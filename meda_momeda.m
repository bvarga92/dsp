clear all;
clc;

%% PARAMETEREK
L=100;        % a szuro hossza
t=0:1e-5:0.1; % idotengely
x=...         % a vizsgalt jel
    sin(2*pi*234*t)+...        % normal periodikus komponens
    0.02*randn(size(t))+...    % zajkomponens
    0.2*square(2*pi*80*t,0.1); % hibakomponens (periodikus utesek)
faultPeriod=1000; % a repetitiv hiba becsult periodusa hany minta (MOMEDA)

%% A TOEPLITZ-MATRIX OSSZEALLITASA
N=length(x);
X0=zeros(L,N-L+1);
for ii=1:L
    X0(ii,:)=x((L-ii+1):(N-ii+1));
end

%% MEDA
figure(1);
subplot(311);
plot(t,x);
xlim([t(1) t(end)]);
title('Eredeti jel');
f=zeros(L,1); f(floor(L/2))=1; f(floor(L/2)+1)=-1;
for ii=1:13
    y=X0'*f;
    subplot(312);
    plot(t,[y ; zeros(length(t)-length(y),1)]);
    xlim([t(1) t(end)]);
    title(sprintf('Szurt jel (MEDA, %d. iteracio)',ii));
    f=sum(y.^2)/sum(y.^4)*inv(X0*X0')*X0*y.^3;
    pause(0.3);
end

%% MOMEDA
target=zeros(N-L+1,1);
target(1:faultPeriod:(N-L+1))=1;
f=inv(X0*X0')*X0*target;
y=X0'*f;
figure(1);
subplot(313);
plot(t,[y ; zeros(length(t)-length(y),1)]);
title('Szurt jel (MOMEDA)');
xlim([t(1) t(end)]);