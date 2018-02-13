clear all;
clc;

%% PARAMETEREK
load('ekg.mat'); %szurt EKG jel (data, N, fs)
L=10;            %integralo ablak hossza
fmax=5;          %ennel magasabb szivfrekvenciat nem varunk (Hz)
RRlim=0.65;      %ha kicsi a csucs, de legalabb ennyivel kovet egy R csucsot, akkor R-nek minositjuk (s)
maxD=50;         %data es y kozti kesleltetes ennel nem nagyobb

%% ELOFELDOLGOZAS
y=-data(1:end-4)-2*data(2:end-3)+2*data(4:end-1)+data(5:end); %derivalas
y=y.^2; %negyzetre emeles
y=filter(ones(1,L)/L,1,y); %mozgoablakos integralas

%% CSUCSOK KERESESE, TUL GYAKORIAK ELTAVOLITASA
peaks=find(([0;diff(y)]>0).*([diff(y);0]<0));
dp=find(diff(peaks)<fs/fmax);
while ~isempty(dp)
    peaks(dp+((y(peaks(dp))-y(peaks(dp+1)))>0))=[];
    dp=find(diff(peaks)<fs/fmax);
end

%% ADAPTIV KUSZOBOZES, QRS CSUCSOK KIVALOGATASA
qrs=1;
SPKI=y(peaks(1));
NPKI=0;
RR(1:5)=RRlim;
for ii=1:length(peaks)
    RRavg=mean(RR);
    THRI(ii)=3/4*NPKI+1/4*SPKI;
    rr=(peaks(ii)-qrs(end))/fs;
    if (y(peaks(ii))>THRI(ii)) || (y(peaks(ii))>THRI(ii)/2 && rr>max(1.5*RRavg,RRlim))
        if rr<RRlim
            d=diff(y(qrs(end):peaks(ii)));
            if max(d)<-0.8*min(d)
                NPKI=7/8*NPKI+1/8*y(peaks(ii));
                continue;
            end
        end
        qrs=[qrs peaks(ii)];
        if(y(peaks(ii))>THRI(ii))
            SPKI=7/8*SPKI+1/8*y(peaks(ii));
        else
            SPKI=3/4*SPKI+1/4*y(peaks(ii));
        end
        if rr>0.9*RRavg && rr<1.2*RRavg
            RR=[rr RR(1:end-1)];
        end
    else
        NPKI=7/8*NPKI+1/8*y(peaks(ii));
    end
end
qrs=qrs(3:end);

%% ELTOLAS A FELDOLGOZASI KESLELTETESSEL
for ii=1:length(qrs)
    l=max(1,qrs(ii)-maxD);
    r=min(N,qrs(ii)+maxD);
    [m ind]=max(data(l:r));
    qrs(ii)=l+ind(1)-1;
end

%% ABRAZOLAS
fprintf('A talalt QRS komplexek szama %u.\n\n',length(qrs));
plot((0:N-1)/fs,data);
hold on;
plot((qrs-1)/fs,data(qrs),'ro','MarkerFaceColor','r');
plot(linspace(0,(N-1)/fs,length(THRI)),THRI,'k','LineWidth',2);
hold off;
xlabel('t [s]');
title('A megtalalt QRS komplexek');
