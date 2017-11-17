clear all;
clc;

N=64;
fs=8000;

h=firpm(N-1,[0 100 300 4000]/fs*2,[1 1 0 0]);
[H,f]=freqz(h,1,5000,fs);
plot(f,20*log10(abs(H)));

file=fopen('coeffs.dat','w');
fprintf(file,'%d,\n',int32(h*(2^15-1)));
fclose(file);
