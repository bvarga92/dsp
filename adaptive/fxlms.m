function [e,w]=fxlms(mu,M,s,c,x,y)
	w=zeros(1,M);
	x_shr=zeros(1,max(M,length(c)));
	y_hat=zeros(1,length(s));
	xc=zeros(1,M);
	e=zeros(1,length(x));
	for ii=1:length(x);
		x_shr=[x(ii) x_shr(1:end-1)];
		y_hat=[-w*x_shr(1:M)' y_hat(1:end-1)];
		e(ii)=y(ii)+s*y_hat';
		xc=[c*x_shr(1:length(c))' xc(1:end-1)];
		w=w+2*mu*xc*e(ii);
	end
