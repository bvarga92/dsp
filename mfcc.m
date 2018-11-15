clear all;
clc;

%% parameterek
fileName='input.wav';
fs=44100;
fL=200;
fH=8000;
numFilters=26;
lift=50;
Tseg=0.025;

%% beolvasas, szegmentalas
x=audioread(fileName);
N=round(Tseg*fs);
Nseg=floor(length(x)/N);
x=x(1:Nseg*N);
segments=reshape(x(:)',N,Nseg)';

%% mel szurobank tervezese
hz_to_mel=@(f) 1127*log(1+f/700);
mel_to_hz=@(m) 700*(exp(m/1127)-1);
freqs=mel_to_hz(linspace(hz_to_mel(fL),hz_to_mel(fH),numFilters+2));
filterBank=zeros(numFilters,floor(N/2));
for ii=2:numFilters+1
    bin1=round(freqs(ii-1)/(fs/N));
    bin2=round(freqs(ii)/(fs/N));
    bin3=round(freqs(ii+1)/(fs/N));
    filterBank(ii-1,bin1:bin2)=((bin1:bin2)-bin1)/(bin2-bin1);
    filterBank(ii-1,bin2:bin3)=(bin3-(bin2:bin3))/(bin3-bin2);
end
figure(1);
plot((0:floor(N/2)-1)/N*fs/1000,filterBank.','k');
axis([0 fs/2000 0 1]);
title('Szurobank');
xlabel('f [kHz]');

%% teljesitmenysuruseg-spektrum eloallitasa
win=hamming(N);
win=win(:)'/sum(win);
win=repmat(win,Nseg,1);
S=abs(fft(segments.*win,N,2)).^2;
S=S(:,1:floor(N/2));

%% Mel-Frequency Cepstrum eloallitasa
MFCC=zeros(Nseg,numFilters);
for ii=1:Nseg
    for jj=1:numFilters
        MFCC(ii,jj)=log10(sum(S(ii,:).*filterBank(jj,:))+1e-20);
    end
    MFCC(ii,:)=dct(MFCC(ii,:));
    if lift>0
        MFCC(ii,:)=MFCC(ii,:).*(1+lift/2*sin(pi/lift*(0:numFilters-1)));
    end
end
figure(2);
subplot(211);
plot((0:length(x)-1)/fs,x);
xlim([0 (length(x)-1)/fs]);
xlabel('t [s]');
subplot(212);
imagesc((0:Nseg-1)*Tseg,1:numFilters,MFCC');
set(gca,'YDir','normal');
xlabel('t [s]');