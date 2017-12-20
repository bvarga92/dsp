%Catmull-Rom-spline interpolacio
    %y:  a jel mintainak vektora
    %x:  az idovektor (hossza megegyezik y hosszaval)
    %x2: ezekben az idopontokban keressuk a jel erteket

function yi=interpolate_spline_cr(y,x,x2)
    yi=zeros(size(x2));
    for ii=1:length(x2)
        idx=sum(x<=x2(ii));
        if idx==1; continue; end;
        if idx==length(x)-1; break; end;
        t=(x2(ii)-x(idx))/(x(idx+1)-x(idx));
        yi(ii)=[1 t t^2 t^3]*...
            [0 1 0 0;-0.5 0 0.5 0;1 -2.5 2 -0.5;-0.5 1.5 -1.5 0.5]*...
            [y(idx-1) ; y(idx) ; y(idx+1) ; y(idx+2)];
    end
