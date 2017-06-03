%Trigonometrikus interpolacio
    %y:  a jel mintainak vektora
    %x:  az idovektor (hossza megegyezik y hosszaval)
    %x2: ezekben az idopontokban keressuk a jel erteket

function yi=interpolate_trig(y,x,x2)
    N=length(y);
    dx=x(2)-x(1);
    Y=fft(y)/N;
    yi=Y(1);
    for ii=1:floor(N/2)
        yi=yi+2*abs(Y(ii+1))*cos(2*pi*ii/dx/N*x2+angle(Y(ii+1)));
    end
