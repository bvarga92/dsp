% Rekurziv lyukszuro.
%    u: a szurendo jel
%    f: az elnyomando relativ frekvenciak vektora (az 1 jelenti a mintavetel frekvenciat)
%    alpha: batorsagi tenyezo
%    e: a szurt jel

function e=rboFilter(u,f,alpha)
    f=f(:)';
    y=zeros(size(u));
    x=zeros(length(f),1);
    e=zeros(size(u));
    for ii=1:length(u)
        c=exp(1j*2*pi*f*(ii-1));
        y(ii)=2*real(c(1:end)*x(1:end));
        e(ii)=u(ii)-y(ii);
        x=x+alpha*e(ii)*c';   
    end