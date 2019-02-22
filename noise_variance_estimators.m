clear all;
clc;

%% noise generation
N=2e4;
switch 5
    case 1 % White Gaussian Noise (WGN)
        noise=randn(1,N);
        name='White Noise';
    case 2 % Random Walk (RW) - integral of WGN
        wn=randn(1,N);
        wn=wn-mean(wn);
        noise=cumsum(wn);
        name='Random Walk';
    case 3 % Random Run (RR) - integral of RW
        wn=randn(1,N);
        rw=cumsum(wn-mean(wn));
        noise=cumsum(rw-mean(rw));
        name='Random Run';
    case 4 % White Phase Noise (WPN) - derivative of WGN
        noise=diff(randn(1,N+1));
        name='White Phase Noise';
    case 5 % Pink Noise (PN, Flicker Noise, 1/f Noise) - Voss algorithm
        noise=zeros(floor(log2(N))+1,N);
        for ii=1:floor(log2(N))+1
            changes=sort(randperm(N,round(N/2^(ii-1))));
            for jj=0:length(changes)
                if jj==0
                    idx=1:changes(1)-1;
                elseif jj==length(changes)
                    idx=changes(end):N;
                else
                    idx=changes(jj):changes(jj+1)-1;
                end
                noise(ii,idx)=randn*ones(1,length(idx));
            end
        end
        noise=sum(noise);
        name='Flicker Noise';
    case 6 % Flicker Walk (FW) - integral of PN
        noise=zeros(floor(log2(N))+1,N);
        for ii=1:floor(log2(N))+1
            changes=sort(randperm(N,round(N/2^(ii-1))));
            for jj=0:length(changes)
                if jj==0
                    idx=1:changes(1)-1;
                elseif jj==length(changes)
                    idx=changes(end):N;
                else
                    idx=changes(jj):changes(jj+1)-1;
                end
                noise(ii,idx)=randn*ones(1,length(idx));
            end
        end
        noise=cumsum(sum(noise));
        name='Flicker Walk';
    case 7 % Flicker Phase Noise (WPN) - derivative of PN
        N=N+1;
        noise=zeros(floor(log2(N))+1,N);
        for ii=1:floor(log2(N))+1
            changes=sort(randperm(N,round(N/2^(ii-1))));
            for jj=0:length(changes)
                if jj==0
                    idx=1:changes(1)-1;
                elseif jj==length(changes)
                    idx=changes(end):N;
                else
                    idx=changes(jj):changes(jj+1)-1;
                end
                noise(ii,idx)=randn*ones(1,length(idx));
            end
        end
        noise=diff(sum(noise));
        N=N-1;
        name='Flicker Phase Noise';
end

%% calculate roll-off and fit line
[psd,w]=pwelch(noise);
a=polyfit(log10(w(2:end)/pi),20*log10(psd(2:end)),1);

%% plot noise and PSD
figure(1);
subplot(121);
plot(noise);
title(name);
subplot(222);
plot(w/pi,20*log10(psd));
hold on;
plot(w/pi,a(1)*log10(w/pi)+a(2),'r','LineWidth',1.5);
hold off;
grid on;
xlabel('f');
ylabel('PSD [dB]');
subplot(224);
semilogx(w/pi,20*log10(psd));
hold on;
semilogx(w/pi,a(1)*log10(w/pi)+a(2),'r','LineWidth',1.5);
hold off;
grid on;
xlabel('f');
ylabel('PSD [dB]');
title(sprintf('Slope: %.f dB/decade',a(1)));
drawnow;

%% calculate variance estimators
Nsamp=1:1:1000;
for ii=1:length(Nsamp)
    noise_M=filter(ones(1,Nsamp(ii))/Nsamp(ii),1,noise); % moving average
    noise_M=noise_M(Nsamp(ii):end); % remove filter transient
    stdvar(ii)=var(noise_M(1:Nsamp(ii):end)); % Standard Variance
    ostdvar(ii)=var(noise_M); % Overlapping Standard Variance
    noise_MD=filter([1 zeros(1,Nsamp(ii)-1) -1],1,noise_M); % differentiation
    noise_MD=noise_MD(Nsamp(ii)+1:end); % remove filter transient
    avar(ii)=0.5*sum(noise_MD(1:Nsamp(ii):end).^2)/length(noise_MD(1:Nsamp(ii):end)); % Allan Variance
    oavar(ii)=0.5*sum(noise_MD.^2)/length(noise_MD); % Overlapping Allan Variance
    noise_MDM=filter(ones(1,Nsamp(ii))/Nsamp(ii),1,noise_MD); % moving average
    noise_MDM=noise_MDM(Nsamp(ii):end); % remove filter transient
    mavar(ii)=0.5*sum(noise_MDM.^2)/length(noise_MDM); % Modified Allan Variance
    noise_MDD=filter([1 zeros(1,Nsamp(ii)-1) -1],1,noise_MD); % differentiation
    noise_MDD=noise_MDD(Nsamp(ii)+1:end); % remove filter transient
    hvar(ii)=0.5*sum(noise_MDD(1:Nsamp(ii):end).^2)/length(noise_MDD(1:Nsamp(ii):end)); % Hadamard Variance
    ohvar(ii)=0.5*sum(noise_MDD.^2)/length(noise_MDD); % Overlapping Hadamard Variance
end

figure(2);
semilogx(Nsamp,stdvar,Nsamp,ostdvar,Nsamp,avar,Nsamp,oavar,Nsamp,mavar,Nsamp,hvar,Nsamp,ohvar);
legend('STDVAR','OSTDVAR','AVAR','OAVAR','MAVAR','HVAR','OHVAR','Location','Best');
xlabel('N_{samp}');
ylabel('\sigma^2');