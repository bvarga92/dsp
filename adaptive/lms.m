function [e,w]=lms(mu,M,u,y)
	w=zeros(1,M);
	x=zeros(1,M);
	e=zeros(1,length(u));
	for ii=1:length(u);
		x=[u(ii) x(1:end-1)];
		e(ii)=y(ii)-w*x';
		w=w+2*mu*x*e(ii);
	end
