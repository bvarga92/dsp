clear all;
clc;

%% parameterek
fs=44100;
N=512;
t=0:1/fs:0.5;
x=sin(2*pi*t*100);
B=5;

%% DCT matrix es ablakfuggveny
M=zeros(N,2*N);
for ii=0:N-1
    M(ii+1,:)=sqrt(2/N)*cos(pi/N*(1/2+ii)*(1/2+N/2+(0:2*N-1)));
end
w=sqrt(2)*sin(pi/2/N*(1/2+(0:2*N-1)));

%% tomorites
x_comp=[];
for ii=1:floor(length(x)/N)-1
    x_comp=[x_comp ; M*(x((ii-1)*N+1:ii*N+N)'.*w')];
end
q=2*max(abs(x_comp))/(2^B-1);
x_comp=round(x_comp/q)*q;

%% kitomorites
for ii=1:floor(length(x)/N)-1
    X=x_comp((ii-1)*N+1:ii*N);
    x_rec=(X'*M.*w);
    if ii==1
        x_uncomp=zeros(1,N);        
    else
        x_uncomp=[x_uncomp (x_rec_prev+x_rec(1:N))/2];
    end
    x_rec_prev=x_rec(N+1:2*N);
end

%% abrazolas
e=x(1:length(x_uncomp))-x_uncomp;
e=e(N+1:end);
figure(1);
subplot(211);
plot(t,x,'b',(0:length(x_uncomp)-1)/fs,x_uncomp,'r');
xlabel('t');
ylabel('x');
subplot(212);
plot(t(N+1:length(x_uncomp)),e);
xlabel('t');
ylabel('e');
figure(2);
plot(x_comp);