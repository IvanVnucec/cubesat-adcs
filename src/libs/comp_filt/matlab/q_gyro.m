function [qib] = q_gyro(w_b, qib_prev, dt)
% Funkcija q_gyro() procijenjuje kvaternion iz ulaznih parametara:
%       w_b = mjereni vektor kutne brzine u koordinatnom sustavu tijela
%       qib_prev = prethodno procijenjeni kvaternion     
%       dt = vremenski korak
% Funkcija q_gd() vraca:
%       qib = estimirani kvaternion


% Racunam matricu za izracun vremenske promjene kvaterniona
S_w = [0     , -w_b(1), -w_b(2), -w_b(3);
       w_b(1),  0     ,  w_b(3), -w_b(2);
       w_b(2), -w_b(3),  0     ,  w_b(1);
       w_b(3),  w_b(2), -w_b(1),  0];  

% Racunam vremeski promjenu (rate of change) kvaterniona iz kutne brzine
d_qib_dt = 0.5*S_w*qib_prev;

% Integriram (akumuliram) promjenu kvaterniona i dodajem prethodnom kvaternionu        
qib = dt*d_qib_dt + qib_prev;

% Normaliziram kvaternion
qib = qib/norm(qib);

% qib predstavlja procjenu kvaterniona u trenutnom koraku
end

