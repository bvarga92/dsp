% AMPD algoritmus (Automatic Multiscale-Based Peak Detection)
% Cikk: Algorithms 2012, 5, 588-603; doi:10.3390/a5040588

clear all;
clc;

%% a jel
N=3000;
x=sin(2*pi*10/800*(0:N-1))+sin(2*pi*70/800*(0:N-1))+0.5*sin(2*pi*5/800*(0:N-1))+0.1*randn(1,N);

%% LMS matrix osszeallitasa
L=ceil(N/2)-1;
M=rand(L,N)+1;
for k=1:L
    for ii=(k+2):(N-k+1)
        if x(ii-1)>x(ii-k-1) && x(ii-1)>x(ii+k-1)
            M(k,ii)=0;
        end
    end
end
figure(1);
subplot(121);
imagesc(M);

%% legtobb csucsot eredmenyezo szelesseg megkeresese
gamma=sum(M,2);
[g, lambda]=min(gamma);
M=M(1:lambda,:);
figure(1);
subplot(122);
imagesc(M);

%% csucsok meghatarozasa az oszloponkenti szoras alapjan
sigma=zeros(1,N);
for ii=1:N
    sigma(ii)=sqrt(1/(lambda-1)*sum((M(:,ii)-mean(M(:,ii))).^2));
end
peaks=find(sigma<eps)-1;
figure(2);
plot(x);
hold on;
plot(peaks,x(peaks),'ro');
hold off;
