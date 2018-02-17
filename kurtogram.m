clear all;
clc;

%% PARAMETEREK
N=4096; % a jel hossza (kettohatvany!)
L=16;   % a FIR szurok hossza
D=5;    % ennyi szinten dekomponalunk (a 0. szint maga a jel)

%% A JEL
x=sin(2*pi*0.002*(0:N-1))+...         % periodikus komponens
  0.1*randn(1,N)+...                  % zaj
  0.5*square(2*pi*0.001*(0:N-1)+3,1); % tranziens zavar
figure(1);
subplot(121);
plot(x);
xlim([0 N-1]);
title('A vizsgalt jel');
xlabel('t');
ylabel('x(t)');

%% SANITY CHECK
if abs(log2(N)-floor(log2(N)))>1e-8
    error('N legyen 2 hatvanya!');
end
if L > N/2^D-1
    error('L < N/2^D-1 legyen! Most %d > %d.',L,N/2^D-1);
end

%% A SZUROK MEGKONSTRUALASA
h=fir1(L-1,0.3);
h0=h.*exp(1j*pi/4*(0:L-1));
h1=h.*exp(1j*3*pi/4*(0:L-1));
figure(2);
plot((0:4095)/4096-0.5,fftshift(abs(fft(h0,4096))),'r');
hold on;
plot((0:4095)/4096-0.5,fftshift(abs(fft(h1,4096))),'b');
hold off;
xlabel('f');
legend('|H_0(f)|','|H_1(f)|');
title('Dekomponalo szurok');

%% A JEL DEKOMPONALASA
C=zeros(D+1,N);
C(1,:)=x;
for ii=1:D
    for jj=1:2:2^ii
        idxSrcStart=(jj-1)*N/2^ii+1;
        idxSrcEnd=(jj+1)*N/2^ii;
        idxDstStart=(jj-1)*N/2^ii+1;
        idxDstEnd=jj*N/2^ii;
        temp=filter(h0,1,C(ii,idxSrcStart:idxSrcEnd));
        C(ii+1,idxDstStart:idxDstEnd)=temp(1:2:end);
        idxDstStart=jj*N/2^ii+1;
        idxDstEnd=(jj+1)*N/2^ii;
        temp=filter(h1,1,C(ii,idxSrcStart:idxSrcEnd)).*(-1j).^(0:(N/2^(ii-1)-1));
        C(ii+1,idxDstStart:idxDstEnd)=temp(1:2:end);
    end
end
figure(3);
imagesc(abs(C));
colorbar;
xl=get(gca,'XLim');
xt=linspace(xl(1),xl(2),6);
set(gca,'XTick',xt);
xt=xt-min(xt);
xt=xt/max(xt)/2;
set(gca,'XTickLabel',xt);
title('Dekomponalt jel');
xlabel('f');
ylabel('szint');

%% A KURTOZISOK SZAMITASA
K=cell(D+1,1);
for ii=0:D
    for jj=1:2^ii
        idxStart=(jj-1)*N/2^ii+1+L; % a tranzienst levagjuk
        idxEnd=jj*N/2^ii;
        temp=C(ii+1,idxStart:idxEnd);
        if all(abs(temp)<eps)
            k=0;
        else
            temp=temp-mean(temp);
            denr=mean(abs(temp).^2);
            if denr<eps
                k=0;
            else
                k=mean(abs(temp).^4)/denr^2-2-1*(ii==0);
            end
        end
        K{ii+1}=[K{ii+1} k*(k>0)];
    end
end

%% ABRAZOLAS
img=zeros(D+1,2^D);
for ii=1:D+1
    temp=repmat(K{ii},2^(D+1-ii),1);
    img(ii,:)=temp(:)';
end
figure(1);
subplot(122);
imagesc(img);
colorbar;
xl=get(gca,'XLim');
xt=linspace(xl(1),xl(2),6);
set(gca,'XTick',xt);
xt=xt-min(xt);
xt=xt/max(xt)/2;
set(gca,'XTickLabel',xt);
title('Kurtogram');
xlabel('f');
ylabel('szint');