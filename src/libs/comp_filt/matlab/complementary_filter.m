%% Program cita autogenerirana mjerenja i izracunava kvaternion
clear all;
close all;
clc;

% -------------------------------------------------------------------------
% Ucitavanje datoteke s generiranim mjerenjima i referentnim vektorima
% -------------------------------------------------------------------------

filename = 'Simulated_measurements.csv';
data_from_file = importdata(filename);
data_from_file = struct2cell(data_from_file);
data    = data_from_file{1};
header  = data_from_file{2};


% -------------------------------------------------------------------------
% Deklaracija i inicijalizacija varijabli
% -------------------------------------------------------------------------

% Maksimalna dopustiva razlika u gresci
dJ_max = 1e-6;
% Maksimalni dopustivi broj iteracija
N_max = 50;
% Stopa konvergencije
alpha = 0.1;
% Inicijalni kvaternion
qib_init = [1 1 1 1]';
qib_init = qib_init/norm(qib_init);
% Frekvencija uzorkovanja u Hz;
fs = 10;
% Vrijeme uzorkovanja
dt = 1/fs;
% Tezinski faktor komplementarnog filtra
K_cf = 0.5;
% Prethodni kvaternion
qib_prev = qib_init;
% Ekstrakcija broja mjerenja
[N_row, N_col] = size(data);
% Definiranje vremenske osi
t = linspace(0, N_row*dt, N_row);

% Ekstrakcija referentnih vektora iz datoteke
a_i = data(1,1:3)'; % Referena akceleracija u inercijalnom sustavu
m_i = data(1,4:6)'; % Referentno magnetsko polje u inercijalnom sustavu


% Ekstrakcija vektora ground thruth kvaterniona iz datoteke
qib_gt_vect = data(:,19:22)';
% Inicijalizacija vektora procijenjenih kvaterniona
qib_est_vect = zeros(4, N_row);
% Inicijalizacija vektora procijenjenih Eulerovih kutova
euler_est_vect = zeros(3, N_row);
% Inicijalizacija vektora ground thruth Eulerovih kutova
euler_gt_vect = zeros(3, N_row);
% Inicijalizacija vektora broja iteracija
NOI_vect = zeros(1, N_row);
% Inicijalizacija vektora RMS greske
qib_RMS_error = zeros(1, N_row);
euler_RMS_error = zeros(1, N_row);

s = comp_filt_init(a_i, m_i, fs, K_cf, dJ_max, N_max, alpha);

% -------------------------------------------------------------------------
% Prolazim kroz uzorke mjerenja u petlji
% -------------------------------------------------------------------------
% for n = 1:5
for n = 1:N_row
    
    % Ekstrakcija mjerenja iz datoteke
    a_b = data(n,10:12)'; % Akceleracija u sustavu tijela
    m_b = data(n,13:15)'; % Magnetsko polje u sustavu tijela
    w_b = data(n,16:18)'; % Kutna brzina u sustavu tijela
    
    [s, qib_est] = comp_filt_step(s, a_b, m_b, w_b);
    
    % Spremanja procijenjenog kvaterniona u vektor
    qib_est_vect(:,n) = qib_est;
end















% Postprocessing i izracun greske

% -------------------------------------------------------------------------
% Izracun RMS greske
% -------------------------------------------------------------------------
for n = 1:N_row
    
    % ---------------------------------------------------------------------
    % Izracun RMS greske kvaterniona
    % ---------------------------------------------------------------------
    
    % Kratko objasnjenje:
    % Ako je kvaternion dobro procijenjen, tada umnozak qib_gt i
    % konjukiranog procijenjenog kvaterniona qib_est_conj daje [1 0 0 0]'.
    % Drugim rijecima, qib_gt rotira u jednom smjeru, a qib_est_conj rotira
    % nazad u suprotnim smjeru od qib_gt. Ako su ta dva kvaterniona
    % jednaka, rezultat ove dvije rotacije je kvaternion [1 0 0 0]'.
    % U realnom slucaju, dobit ce se neki drugi kvaternion ciji se elementi
    % razlikuju od [1 0 0 0]'. Kvaternion greske racunamo kao razliku
    % kvaternion [1 0 0 0]' i umnoska qib_gt i qib_est_conj.
    
    % Detaljnije: Full Quaternion Based Attitude Control for a Quadrotor
    % by Emil Fresk and George Nikolakopoulos

    % Izvlacim procijenjeni kvaternion u n-tom koraku
    qib_est = qib_est_vect(:,n);
    
    % Izvlacim ground thruth kvaterion u n-tim koraku
    qib_gt = qib_gt_vect(:,n);
    
    % Racunam konjugirani kvaternion od procijenjenog kvaterniona
    qib_est_conj = [qib_est(1); -qib_est(2:4)];
      
    % Racunam kvaternion greske
    qib_error = [1,0,0,0]' - quatMul(qib_gt, qib_est_conj);
    
    % Racunam RMS gresku kvateriona i spremam u vektor greske
    qib_RMS_error(n) = sqrt(qib_error'*qib_error/4);
        
    
    % ---------------------------------------------------------------------
    % Izracun RMS greske Eulerovih kutova
    % ---------------------------------------------------------------------
    
    % Racunam procijenjene Eulerove kutove u n-tom koraku
    euler_est = qib2Euler(qib_est);
    
    % Racunam ground thruth Eulerove kutove u n-tom koraku
    euler_gt = qib2Euler(qib_gt);
    
    % Racunam gresku Eulerovih kutova u n-tom koraku
    euler_error = rad2deg(angdiff(deg2rad(euler_est), deg2rad(euler_gt)));
    
    % Racunam RMS gresku Eulerovih kutova i spemam u vektor greske
    euler_RMS_error(n) = sqrt(euler_error'*euler_error/3);
    
    
    % ---------------------------------------------------------------------
    % Spremanje Eulerovih kutova u vektor
    % ---------------------------------------------------------------------
    
    % Spremam procijenjene Eulerove kutove u vektor
    euler_est_vect(:,n) = euler_est;
    
    % Spremam ground thruth Eulerovih kutove u vektor
    euler_gt_vect(:,n) = euler_gt;
end



% ---------------------------------------------------------------------
% Crtanja grafova stvarnih i procijenjenih Eulerovih kutova
% ---------------------------------------------------------------------

Fig1 = figure;
Fig1.Renderer='Painters';

subplot(3,1,1);
plot(t, euler_gt_vect(1,:), t, euler_est_vect(1,:), 'Linewidth', 2);
grid on;
box on;
ylim([-200, 200]);
legend(['~Istiniti kut', '~Procijenjeni kut'], 'Interpreter', 'latex',...
       'Location', 'northeast','FontSize',12);
xlabel('$t$ [s]', 'Interpreter', 'latex','FontSize',20);
ylabel('$\psi$ [deg]', 'Interpreter', 'latex','FontSize',20);
set(gca,'TickLabelInterpreter','latex','FontSize',12);

subplot(3,1,2);
plot(t, euler_gt_vect(2,:), t, euler_est_vect(2,:), 'Linewidth', 2);
grid on;
box on;
legend(['~Istiniti kut', '~Procijenjeni kut'], 'Interpreter', 'latex',...
       'Location', 'northeast','FontSize',12);
xlabel('$t$ [s]', 'Interpreter', 'latex','FontSize',20);
ylabel('$\theta$ [deg]', 'Interpreter', 'latex','FontSize',20);
set(gca,'TickLabelInterpreter','latex','FontSize',12);

subplot(3,1,3);
plot(t, euler_gt_vect(3,:), t, euler_est_vect(3,:), 'Linewidth', 2);
grid on;
box on;
legend(['~Istiniti kut', '~Procijenjeni kut'], 'Interpreter', 'latex',...
       'Location', 'northeast','FontSize',12);
xlabel('$t$ [s]', 'Interpreter', 'latex','FontSize',20);
ylabel('$\phi$ [deg]', 'Interpreter', 'latex','FontSize',20);
set(gca,'TickLabelInterpreter','latex','FontSize',12);




% ---------------------------------------------------------------------
% Crtanja grafova broja iteracija i RMS gresaka
% ---------------------------------------------------------------------
Fig2 = figure;
Fig2.Renderer='Painters';

subplot(2,1,1);
semilogy(t, qib_RMS_error, 'Linewidth', 2);
box on;
grid on;
ylim([1e-4, 1]);
xlabel('$t$ [s]', 'Interpreter', 'latex','FontSize',20);
ylabel('Kvaternion RMSE', 'Interpreter', 'latex','FontSize',20);
set(gca,'TickLabelInterpreter','latex','FontSize',12);

subplot(2,1,2);
semilogy(t, euler_RMS_error, 'Linewidth', 2);
box on;
grid on;
ylim([1e-2, 1e2]);
xlabel('$t$ [s]', 'Interpreter', 'latex','FontSize',20);
ylabel('Euler RMSE', 'Interpreter', 'latex','FontSize',20);
set(gca,'TickLabelInterpreter','latex','FontSize',12);
