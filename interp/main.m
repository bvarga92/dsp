clear all;
clc;

%% EREDETI JEL
N=11;
f=@(x) 1+cos(2*pi*0.1*x)+3*sin(2*pi*0.17*x);
x=0:N-1;
y=f(x);
plot(linspace(0,1,length(y)),y,'bo','MarkerFaceColor','b');
legend('eredeti');
pause;

%% LINEARIS INTERPOLACIO
K=20;
x2=linspace(x(1),x(end),N*K);
yi_lin=interpolate_lin(y,x,x2);
hold on;
plot(linspace(0,1,length(yi_lin)),yi_lin,'b','LineWidth',2);
hold off;
legend('eredeti','linearis');
pause;

%% LAGRANGE-INTERPOLACIO
K=20;
x2=linspace(x(1),x(end),N*K);
yi_lag=interpolate_lag(y,x,x2);
hold on;
plot(linspace(0,1,length(yi_lag)),yi_lag,'r','LineWidth',2);
hold off;
legend('eredeti','linearis','Lagrange');
pause;

%% LEGKISEBB NEGYZETEK MODSZERE, POLINOMIALIS ESET
D=N-1;
K=20;
x2=linspace(x(1),x(end),N*K);
yi_ls=interpolate_ls(y,x,x2,D);
hold on;
plot(linspace(0,1,length(yi_ls)),yi_ls,'g','LineWidth',2);
hold off;
legend('eredeti','linearis','Lagrange','LS');
pause;

%% TRIGONOMETRIKUS INTERPOLACIO
K=20;
x2=linspace(x(1),x(end),N*K);
yi_trig=interpolate_trig(y,x,x2);
hold on;
plot(linspace(0,1,length(yi_trig)),yi_trig,'c','LineWidth',2);
hold off;
legend('eredeti','linearis','Lagrange','LS','Fourier');
pause;

%% HERMITE-INTERPOLACIO
s=2;
K=20;
x2=linspace(x(1),x(end),N*K);
yi_herm=interpolate_herm(y(1:s:end),x(1:s:end),x2);
hold on;
plot(linspace(0,1,length(yi_herm)),yi_herm,'y','LineWidth',2);
hold off;
legend('eredeti','linearis','Lagrange','LS','Fourier','Hermite');
pause;

%% SPLINE INTERPOLACIO
K=20;
x2=linspace(x(1),x(end),N*K);
yi_spline=interpolate_spline(y,x,x2);
hold on;
plot(linspace(0,1,length(yi_spline)),yi_spline,'m','LineWidth',2);
hold off;
legend('eredeti','linearis','Lagrange','LS','Fourier','Hermite','spline');
pause;

%% CATMULL-ROM-SPLINE
K=20;
x2=linspace(x(1),x(end),N*K);
yi_spline_cr=interpolate_spline_cr(y,x,x2);
hold on;
plot(linspace(0,1,length(yi_spline_cr)),yi_spline_cr,'k','LineWidth',2);
hold off;
legend('eredeti','linearis','Lagrange','LS','Fourier','Hermite','spline','C-R-spline');
pause;

%% INTERPOLACIO NULLAK BESZURASAVAL ES ALULATERESZTO SZURESSEL
K=20;
yi_lpf=interpolate_lpf(y,K);
hold on;
plot(linspace(0,1,length(yi_lpf)),yi_lpf,'color',[0.5 0.5 0.5],'LineWidth',2);
hold off;
legend('eredeti','linearis','Lagrange','LS','Fourier','Hermite','spline','C-R-spline','LPF');
