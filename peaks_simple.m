clear all;
clc;

%% PARAMETEREK 
N=3000; % mintaszam
x=sin(2*pi*10/800*(0:N-1))+sin(2*pi*70/800*(0:N-1))+0.5*sin(2*pi*5/800*(0:N-1))+0.1*randn(1,N); % a jel
W=5; % ablakszelesseg
minD=0.5; % csak azokat a csucsokat tartjuk meg, amelyek mindket szomszedjuktol legalabb ennyire elternek

%% CSUCSKERESES
[m, mi]=min(x(1:(1+W)));
[M, Mi]=max(x(1:(1+W)));
mins=[mi m];
maxes=[Mi M];
for ii=(W+1):W:(length(x)-W)
    [m, mi]=min(x(ii:(ii+W)));
    [M, Mi]=max(x(ii:(ii+W)));
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
    if length(m1)==1 && length(m2)==1 % ket minimum egymas utan --> nagyobbat toroljuk
        if mins(m1,2)>mins(m2,2)
            idxRemoveFromMins(end+1)=m1;
        else
            idxRemoveFromMins(end+1)=m2;
        end
    elseif length(M1)==1 && length(M2)==1 % ket maximum egymas utan --> kisebbet toroljuk
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
figure(1);
plot(0:length(x)-1,x);
hold on;
plot(mins(:,1)-1,mins(:,2),'ro');
plot(maxes(:,1)-1,maxes(:,2),'go');
hold off;