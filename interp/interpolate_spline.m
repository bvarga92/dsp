%Harmadfoku, elsorendu spline interpolacio
    %y:  a jel mintainak vektora
    %x:  az idovektor (hossza megegyezik y hosszaval)
    %x2: ezekben az idopontokban keressuk a jel erteket

function yi=interpolate_spline(y,x,x2)
    dy=(y(2:end)-y(1:end-1))./(x(2:end)-x(1:end-1));
    yi=[];
    for ii=1:length(x)-2;
        L=x(ii);
        R=x(ii+1);
        A=[1  L  L^2  L^3;
           1  R  R^2  R^3;
           0  1  2*L  3*L^2;
           0  1  2*R  3*R^2;];
        b=[y(ii) ; y(ii+1) ; dy(ii) ; dy(ii+1)];
        v=A\b;
        xi=x2(x2>=L & x2<=R);
        yi=[yi , v(1)+v(2)*xi+v(3)*xi.^2+v(4)*xi.^3];
    end
    yi=[yi , yi(end)*ones(1,length(x2)-length(yi))];
