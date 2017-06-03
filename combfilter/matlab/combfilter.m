clear all;
clc;

% ----------------------- BEMENET ----------------------- %
[u fs B]=wavread('input2.wav');

% --------------------- PARAMETEREK --------------------- %
M=9600; FF=0; FB=0.2; BL=1; %echo
%M=2400; FF=0.5; FB=0; BL=1; %slapback

% ----------------------- SZAMITAS ---------------------- %
disp(sprintf('%.1f kHz, %d bit, %.1f s',fs/1000,B,length(u)/fs));
delayLine=zeros(1,M);
y=zeros(1,length(u));
for ii=1:length(u)
    temp=u(ii)+FB*delayLine(M);
    y(ii)=FF*delayLine(M)+BL*temp;
    delayLine=[temp delayLine(1:end-1)];
end
sound(u,fs);
sound(y,fs);
