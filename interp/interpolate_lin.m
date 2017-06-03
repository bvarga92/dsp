%Linearis interpolacio
    %y:  a jel mintainak vektora
    %x:  az idovektor (hossza megegyezik y hosszaval)
    %x2: ezekben az idopontokban keressuk a jel erteket

function yi=interpolate_lin(y,x,x2)
    yi=zeros(size(x2));
    for ii=1:length(x2)
        if x2(ii)>=x(end)
            yi(ii)=y(end);
        else
            i1=find(x<=x2(ii));
            i1=i1(end);
            i2=i1+1;
            yi(ii)=(x2(ii)-x(i1))/(x(i2)-x(i1))*(y(i2)-y(i1))+y(i1);
        end
    end
