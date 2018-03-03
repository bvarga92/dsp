clear all;
clc;

%% PARAMETEREK
fs=1e5; % mintaveteli frekvencia
t=0:1/fs:0.5; % idovektor
x=sin(2*pi*210*t)+0.1*sin(2*pi*80*t)+0.03*sin(2*pi*70*t); % jel
W=10; % ablakszelesseg a csucskereseshez
minD=5e-4; % csak azokat a csucsokat tartjuk meg, amelyek mindket szomszedjuktol legalabb ennyivel elternek
imfs=3; % hany IMF-et szamoljunk ki
iters=4; % IMF-enkenti iteracioszam

%% EMPIRICAL MODE DECOMPOSITION
x2=x;
for imf=1:imfs
    h=x2;
    for iter=1:iters
        % CSUCSKERESES
        [m, mi]=min(h(1:(1+W)));
        [M, Mi]=max(h(1:(1+W)));
        mins=[mi m];
        maxes=[Mi M];
        for ii=(W+1):W:(length(h)-W)
            [m, mi]=min(h(ii:(ii+W)));
            [M, Mi]=max(h(ii:(ii+W)));
            newMin=(mins(end,1)~=mi+ii-1 && maxes(end,1)~=mi+ii-1);
            newMax=(maxes(end,1)~=Mi+ii-1 && mins(end,1)~=Mi+ii-1);
            if newMin
                mins=[mins; mi+ii-1 m];
            end
            if newMax
                maxes=[maxes ; Mi+ii-1 M];
            end
        end
        idx=sort([mins(:,1) ; maxes(:,1)]);
        idxRemoveFromMaxes=[];
        idxRemoveFromMins=[];
        for ii=1:length(idx)-1
            m1=find(mins(:,1)==idx(ii));
            m2=find(mins(:,1)==idx(ii+1));
            M1=find(maxes(:,1)==idx(ii));
            M2=find(maxes(:,1)==idx(ii+1));
            if length(m1)==1 && length(m2)==1
                if mins(m1,2)>mins(m2,2)
                    idxRemoveFromMins(end+1)=m1;
                else
                    idxRemoveFromMins(end+1)=m2;
                end
            elseif length(M1)==1 && length(M2)==1
                if maxes(M1,2)<maxes(M2,2)
                    idxRemoveFromMaxes(end+1)=M1;
                else
                    idxRemoveFromMaxes(end+1)=M2;
                end
            end
        end
        mins(idxRemoveFromMins,:)=[];
        maxes(idxRemoveFromMaxes,:)=[];
        [idx, ord]=sort([mins(:,1) ; maxes(:,1)]);
        peaks=[mins(:,2) ; maxes(:,2)];
        peaks=peaks(ord);
        idxRemoveFromMaxes=[];
        idxRemoveFromMins=[];
        if peaks(1)>peaks(2)
            idxRemoveFromMaxes(end+1)=1;
        else
            idxRemoveFromMins(end+1)=1;
        end
        if peaks(end)>peaks(end-1)
            idxRemoveFromMaxes(end+1)=size(maxes,1);
        else
            idxRemoveFromMins(end+1)=size(mins,1);
        end
        for ii=2:length(peaks)-1
            d1=peaks(ii)-peaks(ii-1);
            d2=peaks(ii)-peaks(ii+1);
            if abs(d1)<minD || abs(d2)<minD
                if d1<0
                    idxRemoveFromMins(end+1)=find(mins(:,1)==idx(ii));
                else
                    idxRemoveFromMaxes(end+1)=find(maxes(:,1)==idx(ii));           
                end
            end
        end
        mins(idxRemoveFromMins,:)=[];
        maxes(idxRemoveFromMaxes,:)=[];
        % HARMADFOKU, ELSORENDU SPLINE A LOKALIS SZELSOERTEKEKRE
        vx=[t(1) ; mins(:,1) ; t(end)];
        vy=[x(1) ; mins(:,2) ; x(end)];
        dy=(vy(2:end)-vy(1:end-1))./(vx(2:end)/fs-vx(1:end-1)/fs);
        envL=[];
        for ii=1:length(vx)-2;
            L=vx(ii)/fs;
            R=vx(ii+1)/fs;
            A=[1  L  L^2  L^3;
               1  R  R^2  R^3;
               0  1  2*L  3*L^2;
               0  1  2*R  3*R^2;];
            b=[vy(ii) ; vy(ii+1) ; dy(ii) ; dy(ii+1)];
            v=A\b;
            xi=t(t>=L & t<=R);
            envL=[envL , v(1)+v(2)*xi+v(3)*xi.^2+v(4)*xi.^3];
        end
        if length(envL)<length(t)
            envL=[envL , envL(end)*ones(1,length(t)-length(envL))];
        else
            envL=envL(1:length(t));
        end
        vx=[t(1) ; maxes(:,1) ; t(end)];
        vy=[x(1) ; maxes(:,2) ; x(end)];
        dy=(vy(2:end)-vy(1:end-1))./(vx(2:end)/fs-vx(1:end-1)/fs);
        envU=[];
        for ii=1:length(vx)-2;
            L=vx(ii)/fs;
            R=vx(ii+1)/fs;
            A=[1  L  L^2  L^3;
               1  R  R^2  R^3;
               0  1  2*L  3*L^2;
               0  1  2*R  3*R^2;];
            b=[vy(ii) ; vy(ii+1) ; dy(ii) ; dy(ii+1)];
            v=A\b;
            xi=t(t>=L & t<=R);
            envU=[envU , v(1)+v(2)*xi+v(3)*xi.^2+v(4)*xi.^3];
        end
        if length(envU)<length(t)
            envU=[envU , envU(end)*ones(1,length(t)-length(envU))];
        else
            envU=envU(1:length(t));
        end
        % KOMPENZALAS A BURKOLOGORBEK ATLAGAVAL
        m=(envU+envL)/2;
        figure(imf);
        plot(t,h,'b',t,envU,'g',t,envL,'r',t,m,'k',maxes(:,1)/fs,maxes(:,2),'g.',mins(:,1)/fs,mins(:,2),'r.');
        xlabel('t');
        ylabel(sprintf('h_%d(t)',imf));
        title(sprintf('%d. iteracio',iter));
        pause(1);
        h=h-m;
    end
    x2=x2-h;
end