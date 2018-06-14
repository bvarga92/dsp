clear all;
clc;

%% parameterek
N=1000;      % ennyi mintat szimulalunk
y=@(x) filter(1./(10:10:100),1,x)+0.5*filter(1./(10:10:100),1,x).^2; % a nemlinearis rendszer
lambda=0.95; % RLS felejtesi faktor (0<lambda<=1)
L=15;        % a Volterra-modell memoriajanak hossza
L_lms=15;    % LMS algoritmussal illesztett linearis modell egyutthatoszama 
mu_lms=1e-2; % LMS algoritmus batorsagi tenyezoje

%% Volterra-kernelek identifikacioja RLS algoritmussal
x=randn(1,N); % gerjesztes
y_=y(x); % a nemlinearis rendszer valasza a gerjesztesre
h=zeros(1,L+L*(L+1)/2); % [h1(0) ... h1(L-1)  h2(0,0) h2(0,1)     ... h2(L-2,L-1)       h2(L-1,L-1)]
X=zeros(1,L+L*(L+1)/2); % [x(n)  ... x(n-L+1) x^2(n)  x(n)*x(n-1) ... x(n-L+2)*x(n-L+1) x^2(n-L+1)]
P=1000*eye(L+L*(L+1)/2);
e=zeros(1,N);
for ii=L:N
    X(1:L)=x(ii:-1:ii-L+1);
    for xi=0:L-1
        for xj=xi:L-1
            X((L-xi/2).*(xi+1)+xj+1)=x(ii-xi)*x(ii-xj);
        end
    end
    e(ii)=y_(ii)-X*h';
    P=1/lambda*(P-P*(X')*X*P/(lambda+X*P*X'));
    h=h+X*P'*e(ii);
end

%% osszehasonlitaskeppen megprobalunk egy sima linearis LMS-t is
h=zeros(1,L_lms);
X=zeros(1,L_lms);
e_lms=zeros(1,N);
for ii=L_lms:N
    X=x(ii:-1:ii-L_lms+1);
    e_lms(ii)=y_(ii)-X*h';
    h=h+mu_lms*e_lms(ii)*X;
end

%% abrazolas
figure(1);
subplot(211);
plot(0:N-1,e);
xlim([0 N-1]);
xlabel('n');
ylabel('e(n)');
title('Identifikacio masodfoku Volterra-sorral');
subplot(212);
plot(0:N-1,e_lms);
xlim([0 N-1]);
xlabel('n');
ylabel('e(n)');
title('Identifikacio linearis szurovel');