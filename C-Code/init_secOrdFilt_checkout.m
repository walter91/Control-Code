% init_secOrdFilt_checkout
mex -g cfun_secOrdFilt_checkout.c CNTR_UTIL.c
SIM_dt = 0.01;

zeta = 0.250;
w0 = 3*2*pi;

num = [0,0,w0^2];
den = [1,2*zeta*w0,w0^2];

[FILT] = c2d(tf(num,den),SIM_dt,'tustin');

 
a0 = FILT.Numerator{1}(1);
a1 = FILT.Numerator{1}(2);
a2 = FILT.Numerator{1}(3);

b0 = FILT.Denominator{1}(1);
b1 = FILT.Denominator{1}(2);
b2 = FILT.Denominator{1}(3);


sim('secOrdFilt_checkout');