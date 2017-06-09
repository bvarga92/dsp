clear all;
clc;

N=1199;
fs=48000;

h=firpm(N-1,[0 4000 4100 24000]/fs*2,[1 1 0 0]);
[H,f]=freqz(h,1,5000,fs);
plot(f,20*log10(abs(H)));
file=fopen('coeffsL.txt','w');
fprintf(file,'%.20f,\n',h);
fclose(file);

h=firpm(N-1,[0 4000 4100 24000]/fs*2,[0 0 1 1]);
[H,f]=freqz(h,1,5000,fs);
hold on;
plot(f,20*log10(abs(H)),'r');
hold off;
file=fopen('coeffsR.txt','w');
fprintf(file,'%.20f,\n',h);
fclose(file);
