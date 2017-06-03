function [e,w]=nlms(mu,a,M,u,y)
	w=zeros(1,M);
	x=zeros(1,M);
	e=zeros(1,length(u));
	for ii=1:length(u);
		x=[u(ii) x(1:end-1)];
		e(ii)=y(ii)-w*x';
		w=w+mu*x*e(ii)/(a+x*x');
	end
