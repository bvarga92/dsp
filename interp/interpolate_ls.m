%Polinomialis Least Squares interpolacio
    %y:  a jel mintainak vektora
    %x:  az idovektor (hossza megegyezik y hosszaval)
    %x2: ezekben az idopontokban keressuk a jel erteket
    %D:  az illesztendo polinom fokszama

function yi=interpolate_ls(y,x,x2,D)
    X=ones(length(y),D+1);
    for ii=1:D
        X(:,ii+1)=x(:).^ii;
    end
    A=X\y(:);
    yi=A(1)*ones(size(x2));
    for ii=1:D
        yi=yi+A(ii+1)*x2.^ii;
    end
