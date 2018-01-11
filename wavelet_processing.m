clear all;
clc;

%% PARAMETEREK
fs=1e5;       % mintaveteli frekvencia
t=0:1/fs:1;   % ido
x=chirp(t,1,t(end),50)+5*(square(2*pi*15*t+2*pi*rand,2)+1)+randn(size(t)); % a vizsgalt jel
wname='haar'; % mother wavelet tipusa
levels=7;     % hany szintu transzformaciot szeretnenk csinalni

%% WAVELET ANALIZIS
figure(1);
subplot(2,1,1);
plot(x);
xlim([0 length(x)-1]);
title('Original');
figure(2);
A=x;
for ii=1:levels
    [A,D]=dwt(A,wname);
    subplot(levels,2,2*ii-1);
    plot(A);
    xlim([0 length(A)-1]);
    title(sprintf('Level %d, approx.',ii));
    subplot(levels,2,2*ii);
    plot(D);
    xlim([0 length(D)-1]);
    title(sprintf('Level %d, detail',ii));
end
xr=idwt(A,D,wname);
figure(1);
subplot(2,1,2);
plot(xr);
xlim([0 length(xr)-1]);
title(sprintf('Reconstructed from level %d',levels));
