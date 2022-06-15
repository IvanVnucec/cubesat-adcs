function [ s ] = comp_filt_init( acc_i, mag_i, fs, K_cf, dJ_max, N_max, alpha )
%COMP_FILT_INIT Summary of this function goes here
%   Detailed explanation goes here

s.acc_i = acc_i / norm(acc_i);
s.mag_i = mag_i / norm(mag_i);
s.fs = fs;
s.K_cf = K_cf;
s.dJ_max = dJ_max;
s.N_max = N_max;
s.alpha = alpha;

s.dt = 1 / fs;

qib_prev = [1 1 1 1]';
s.qib_prev = qib_prev / norm(qib_prev);

end
