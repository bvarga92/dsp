clear all;
clc;

string='Hello world! :)'; %ezt kodoljuk
filename='fsk.wav'; %kimeneti fajlnev
f0=1000; %0 bithez tartozo frekvencia
f1=5000; %1 bithez tartozo frekvencia
fs=48000; %mintaveteli frekvencia
bittime=1000; %bitido ennyi minta
bitpause=500; %bitkozi szunet ennyi minta
charpause=2000; %karakterkozi szunet ennyi minta

len=length(string);
ascii=double(string)'
fsk=zeros(1,charpause);
for char=1:len
    for bit=1:8
        if bitget(ascii(char),bit)==0
            fsk=[fsk,0.9*sin(2*pi*f0/fs*(0:bittime-1)),zeros(1,bitpause)];
        else
            fsk=[fsk,0.9*sin(2*pi*f1/fs*(0:bittime-1)),zeros(1,bitpause)];
        end
    end
    fsk=[fsk zeros(1,charpause-bitpause)];
end
plot((0:length(fsk)-1)/fs,fsk);
wavwrite(fsk,fs,filename);
