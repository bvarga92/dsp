clear all;
clc;

%% parameterek
N=1000;      % ennyi mintat szimulalunk
y=@(x) filter(1./(10:10:100),1,x)+filter(1./(10:10:100),1,x).^2+1.5*filter(1./(10:10:100),1,x).^3; % a nemlinearis rendszer
ord=3;       % az illesztett Volterra-modell fokszama
lambda=0.98; % RLS felejtesi faktor (0<lambda<=1)
L=10;        % a Volterra-modell memoriajanak hossza

%% Volterra-kernelek identifikacioja RLS algoritmussal
x=randn(1,N); % gerjesztes
y_=y(x); % a nemlinearis rendszer valasza a gerjesztesre
K=(ord+1)*nchoosek(ord+L,ord+1)/L-1; % ennyi egyutthatora lesz szukseg (ismetleses kombinaciok szama)
h=zeros(1,K);
X=zeros(1,K);
P=1000*eye(K);
e=zeros(1,N);
for ii=L:N
    pos=1;
    for jj=1:ord
        combs=bsxfun(@minus,nchoosek(0:L+jj-2,jj),0:jj-1);
        X(pos:pos+size(combs,1)-1)=prod(reshape(x(ii-combs),size(combs)),2);
        pos=pos+size(combs,1);
    end
    e(ii)=y_(ii)-X*h';
    P=1/lambda*(P-P*(X')*X*P/(lambda+X*P*X'));
    h=h+X*P'*e(ii);
end

%% abrazolas
figure(1);
plot(0:N-1,e);
xlim([0 N-1]);
xlabel('n');
ylabel('e(n)');