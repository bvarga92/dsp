%Lagrange-interpolacio
    %y:  a jel mintainak vektora
    %x:  az idovektor (hossza megegyezik y hosszaval)
    %x2: ezekben az idopontokban keressuk a jel erteket

function yi=interpolate_lag(y,x,x2)
    yi=zeros(size(x2));
    for ii=1:length(x)
        P=ones(size(x2));
        for jj=1:length(x)
            if jj~=ii
                P=P.*(x2-x(jj))/(x(ii)-x(jj));
            end
        end
        yi=yi+y(ii)*P;
    end
