clear all;
clc;

acc_i = [0 0 -9.81]';     % ref. inercijski u NED sustavu
mag_i = [22.2 1.7 42.7]'; % ref. inercijski u NED sustavu
fs = 100;   % Hz
K_cf = 0.9; % Tezinski faktor komplementarnog filtra
dJ_max = 5e-3; % Maksimalna dopustiva razlika u gresci
N_max = 10; % Maksimalni dopustivi broj iteracija
alpha = 0.1; % Stopa konvergencije

s = comp_filt_init(acc_i, mag_i, fs, K_cf, dJ_max, N_max, alpha);

for i = 1 : 10
    % get measurements (in NED)
    acc_b = [0 0 -9.81]';
    mag_b = [22.2 1.7 42.7]';
    gyr_b = [0 0 0]';
    
    qib_est = comp_filt_step(s, acc_b, mag_b, gyr_b);
end
