function [ qib_est ] = comp_filt_step( s, acc_b, mag_b, gyr_b )
%COMP_FILT_STEP Summary of this function goes here
%   Detailed explanation goes here

% Normalize measurements
acc_b = acc_b / norm(acc_b);             	% normalized column vector in NED
mag_b = mag_b / norm(mag_b);             	% normalized row vector in NED

% Izracunavam kvaternion iz ziroskopa
qib_gyro = q_gyro(gyr_b, s.qib_prev, s.dt);

% Racunam optimalni kvaternon iz acc i mag pomocu gradient descenta
% Za inicijalni kvaternion koristim onaj izracunat iz ziroskopa
[qib_gd, ~] = q_gd(acc_b, mag_b, s.acc_i, s.mag_i, qib_gyro, s.alpha, s.N_max, s.dJ_max);

% Komplementarni filtar
qib_est = s.K_cf * qib_gyro + (1 - s.K_cf) * qib_gd;
qib_est = qib_est / norm(qib_est);

% Storing current quaternion for the next iterative process
s.qib_prev = qib_est; 

end
