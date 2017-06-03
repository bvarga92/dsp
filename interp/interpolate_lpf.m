%Interpolacio nullak beszurasaval es alulatereszto szuressel
    %y: a jel mintainak vektora
    %K: az interpolacios arany

function yi=interpolate_lpf(y,K)
    y=y(:)';
    yi=[y ; zeros(K-1,length(y))];
    yi=yi(:)';
    [B,A]=butter(6,0.8/K);
    yi=filter(B,A,yi)*K;
