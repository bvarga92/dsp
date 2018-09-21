clear all;
clc;

%% parameterek
useParallel=true; % hasznaljunk-e parhuzamos vegrehajtast?
closePool=true;   % bezarjuk-e a poolt a munka vegeztevel?
params=[
    struct(                             ...
        'A11',2,'Delta11',5,            ...
        'A12',1,'Delta12',0,            ...
        'A21',1,'Delta21',0,            ...
        'A22',2,'Delta22',0,            ...
        'pts',100                       ...
    )
    struct(                             ...
        'A11',2,'Delta11',10,           ...
        'A12',1,'Delta12',0,            ...
        'A21',1,'Delta21',0,            ...
        'A22',2,'Delta22',0,            ...
        'pts',100                       ...
    )
    struct(                             ...
        'A11',2,'Delta11',20,           ...
        'A12',1,'Delta12',0,            ...
        'A21',1,'Delta21',0,            ...
        'A22',2,'Delta22',0,            ...
        'pts',100                       ...
    )
    struct(                             ...
        'A11',2,'Delta11',40,           ...
        'A12',1,'Delta12',0,            ...
        'A21',1,'Delta21',0,            ...
        'A22',2,'Delta22',0,            ...
        'pts',100                       ...
    )
];

%% worker pool letrehozasa
if useParallel
    if verLessThan('matlab','8.3.0.532')
        if matlabpool('size')==0
            matlabpool('open',length(params));
        end
    else
        if isempty(gcp('nocreate'))
            parpool('local',length(params));
        end
    end
end

%% szamitas
tic;
syms z;
parfor p=1:length(params)
    A11=params(p).A11; Delta11=params(p).Delta11;
    A12=params(p).A12; Delta12=params(p).Delta12;
    A21=params(p).A21; Delta21=params(p).Delta21;
    A22=params(p).A22; Delta22=params(p).Delta22;
    omega0=linspace(0.001,pi-0.001,params(p).pts);
    Kmax=zeros(size(omega0)); % itt K=mu*C^2*L
    S11=A11*z^-Delta11;
    S12=A12*z^-Delta12;
    S21=A21*z^-Delta21;
    S22=A22*z^-Delta22;
    str=[];
    for ii=1:length(omega0)
        if ~useParallel
            fprintf(repmat('\b',1,length(str)));
            str=sprintf('%d./%d abra, %d./%d pont.\n',p,length(params),ii,length(omega0));
            fprintf(str);
        end
        Phi11=-omega0(ii)*Delta11;
        Phi12=-omega0(ii)*Delta12;
        Phi21=-omega0(ii)*Delta21;
        Phi22=-omega0(ii)*Delta22;
        K=1;
        step=1;
        while step>1e-5
            alpha11=K*A11*(cos(omega0(ii)-Phi11)*z-cos(Phi11))/(z^2-2*cos(omega0(ii))*z+1);
            alpha12=K*A12*(cos(omega0(ii)-Phi12)*z-cos(Phi12))/(z^2-2*cos(omega0(ii))*z+1);
            alpha21=K*A21*(cos(omega0(ii)-Phi21)*z-cos(Phi21))/(z^2-2*cos(omega0(ii))*z+1);
            alpha22=K*A22*(cos(omega0(ii)-Phi22)*z-cos(Phi22))/(z^2-2*cos(omega0(ii))*z+1);
            % rts=solve(1+alpha11*S11+alpha12*S12+alpha21*S21+alpha22*S22+(alpha11*alpha22-alpha12*alpha21)*(S11*S22-S12*S21)==0,z); % ez nagyon lassu
            rts=roots(sym2poly(collect((1+alpha11*S11+alpha12*S12+alpha21*S21+alpha22*S22+(alpha11*alpha22-alpha12*alpha21)*(S11*S22-S12*S21))*(z^2-2*cos(omega0(ii))*z+1)^2*z^max(Delta11+Delta22,Delta12+Delta21))));
            step=step/2;
            if any(abs(rts)>=1)
                K=K-step;
            else
                K=K+step;
            end
        end
        Kmax(ii)=K;
    end
    fig=figure(p);
    plot(omega0,Kmax);
    axis([minmax(omega0) minmax(Kmax)]);
    xlabel('\omega_0');
    ylabel('K_{max}');
    title(sprintf('A_{11}=%.1f, \\Delta_{11}=%d, A_{12}=%.1f, \\Delta_{12}=%d, A_{21}=%.1f, \\Delta_{21}=%d, A_{22}=%.1f, \\Delta_{22}=%d',A11,Delta11,A12,Delta12,A21,Delta21,A22,Delta22));
    saveas(fig,sprintf('%d',p),'fig');
    close(fig);
end
toc

%% abrak megnyitasa es konvertalasa PNG-be (parfor-ban nem lehet abrazolni)
for p=1:length(params)
    openfig(sprintf('%d.fig',p));
    saveas(gcf,sprintf('%d',p),'png');
end

%% worker pool bezarasa
if useParallel && closePool
    if verLessThan('matlab','8.3.0.532')
        matlabpool('close');
    else
        delete(gcp('nocreate'));
    end
end