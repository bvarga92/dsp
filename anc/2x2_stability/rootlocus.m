clear all;
clc;

omega0=0.00001;
% omega0=pi-0.00001;
Delta=15;
K=logspace(-2,1,100);

Num=[zeros(1,Delta+1)   cos(omega0*(Delta+1))   -cos(omega0*Delta)];
Den=[1   -2*cos(omega0)   1   zeros(1,Delta)];

figure(1);
clf;
rectangle('Position',[-1 -1 2 2],'Curvature',[1 1]);
hold on;
axis([-1.5 1.5 -1.5 1.5]);
grid on;
axis square;
found=false;
for ii=1:length(K)
    rts=roots(Den+K(ii)*Num);
    plot(real(rts),imag(rts),'.','color',[ii/length(K) 0 1-ii/length(K)]);
    if found==false && any(abs(rts)>=1)
        fprintf('A stabilitas elveszett. Polusok:\n\n');
        for jj=1:length(rts)
            fprintf('    |%+f %+fj| = %f\n',real(rts(jj)),imag(rts(jj)),abs(rts(jj)));
        end
        fprintf('\nKmax=%f\n\n',K(ii-1));
        fprintf('Elmeleti Kmax a szeleken: %f\n\n',2/(2*Delta+1));
        found=true;
    end
    pause(0.01);
end
hold off;