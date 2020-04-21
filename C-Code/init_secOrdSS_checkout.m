% init_secOrdFilt_checkout
clear;
clear mex;

%%
mex -g cfun_secOrdSS.c CNTR_UTIL.c

 %%
SIM_dt = 0.001;
% 
% zeta = 0.250;
% w0 = 3*2*pi;
% 
% num = [0,0,w0^2];
% den = [1,2*zeta*w0,w0^2];
% 
% [FILT] = c2d(tf(num,den),SIM_dt,'tustin');
% 
% FILTss = c2d(ss(tf(num,den)), SIM_dt,'tustin')
% 
%  
% 
% [A,B,C,D] = ssdata(FILTss);
% 
% C = [C;0,0];
% D = [D;0];
% 
% A00 = A(1,1);
% A10 = A(2,1);
% A01 = A(1,2);
% A11 = A(2,2);
% 
% B00 = B(1,1);
% B10 = B(2,1);
% 
% C00 = C(1,1);
% C10 = C(2,1);
% C01 = C(1,2);
% C11 = C(2,2);
% 
% D00 = D(1,1);
% D10 = D(2,1);

%%
zeta = 0.707;
wn = 10*2*pi;

% M = 1;
% K = (wn^2)*M;
% B = 2*zeta*wn*M;

num = [0,0,wn^2];
den = [1,2*zeta*wn,wn^2];

sysTF_c = tf(num,den);
sysSS_c = ss(sysTF_c);

sysTF = c2d(sysTF_c,SIM_dt,'Tustin');

num = sysTF.Numerator;
den = sysTF.Denominator;

sysSS = ss(sysTF);
A = sysSS.A;
B = sysSS.B;
C = sysSS.C;
D = sysSS.D;

if 0
A00 = sysSS_c.A(1,1);
A01 = sysSS_c.A(1,2);
A10 = sysSS_c.A(2,1);
A11 = sysSS_c.A(2,2);

% A = [A00, A01; A10, A11]

B00 = sysSS_c.B(1,1);
B10 = sysSS_c.B(2,1);

% B = [B00; B10]

C00 = sysSS_c.C(1,1);
C01 = sysSS_c.C(1,2);
C10 = 0;
C11 = 0;

% C = [C00, C01; C10, C11]

D00 = sysSS_c.D(1,1);
D10 = 0;

% D = [D00; D10]

else
A00 = A(1,1);
A01 = A(1,2);
A10 = A(2,1);
A11 = A(2,2);

% A = [A00, A01; A10, A11]

B00 = B(1,1);
B10 = B(2,1);

% B = [B00; B10]

C00 = C(1,1);
C01 = C(1,2);
C10 = 0;
C11 = 0;

% C = [C00, C01; C10, C11]

D00 = D(1,1);
D10 = 0;

% D = [D00; D10]
end


sim('secOrdSS_checkout');