%Hermite-interpolacio
    %y:  a jel mintainak vektora
    %x:  az idovektor (hossza megegyezik y hosszaval)
    %x2: ezekben az idopontokban keressuk a jel erteket

function yi=interpolate_herm(y,x,x2)
    dy=(y(2:end)-y(1:end-1))./(x(2:end)-x(1:end-1));
    N=length(x);
    A=zeros(2*N-1,2*N-1);
    b=zeros(2*N-1,1);
    for ii=1:N;
        A(ii+1,:)=x(ii).^(0:2*N-2);
        b(ii+1)=y(ii);
    end
    for ii=1:N-1;
        A(N+ii+1,:)=[0 (1:2*N-2).*(x(ii).^(0:2*N-3))];
        b(N+ii+1)=dy(ii);
    end
    v=A\b;
    yi=0;
    for ii=0:length(v)-1;
        yi=yi+v(ii+1)*x2.^ii;
    end
