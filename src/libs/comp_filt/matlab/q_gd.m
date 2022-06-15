function [qib, NOI] = q_gd(a_b, m_b, a_i, m_i, qib, alpha, N_max, dJ_max)
% Funkcija q_gd() je iterativni estimator kvaterniona koja prima:
%       a_b = mjereni vektor akceleracije u koordinatnom sustavu tijela
%       m_b = mjereni vektor magnetskog polja u koordinatnom sustavu tijela     
%       a_i = mjereni vektor akceleracije u inercijalnom koordinatnom sustavu
%       m_i = mjereni vektor akceleracije u inercijalnom koordinatnom sustavu
%       qib = inicijalni kvaternion za poèetak iteratovnog postupa te maksimalnu
%       N_max = maksimalni broj iteracija
%       dJ_max = dozvoljenu razliku u gresci
% Funkcija q_gd() vraca vektor koji sadrzi dva elementa:
%       qib = estimirani kvaternion
%       NOI = broj izvršenih itreracija

a_i = a_i/norm(a_i);    % Normaliziram akceleraciju
m_i = m_i/norm(m_i);    % Normaliziram mag polje

a_b = a_b/norm(a_b);    % Normaliziram akceleraciju
m_b = m_b/norm(m_b);    % Normaliziram mag polje

J_previous = 0;         % Prethodna vrijednost funkcije cilja (geldamo razliku)
NOI = 0;                % Trenutni broj iteracija

% Pokrecem petlju s maksimalno N_max iteracija
for i=1:N_max
    
    % Racunam rotacijsku matricu
    Rib = qib2Rib(qib);
    
    % Racunam vektora greske
    e_a = Rib*a_i - a_b;
    e_m = Rib*m_i - m_b;
    
    % Racunam vrijednost funckije cilja u i-toj iteraciji
    J_a = e_a'*e_a;
    J_m = e_m'*e_m;
    J_current = J_a + J_m;
    
    % Provjeravam je li ispunjen uvjet za prekid iterativnog postupka
    if abs(J_current - J_previous) < dJ_max
        break;
    end
        
    % Racunam gradijent funkcije cilja za trenutni kvaternion
    grad_J = grad(a_i, m_i, e_a, e_m, qib);

    % Primijenjujem gradient decent algoritam na svaki element kvaterniona
    qib = qib - alpha*grad_J;
    
    % Normaliziram izraèunati kvaternion
    qib = qib/norm(qib);
    
    % Pripremam vrijednost cost funkcije za sljedecu iteraciju
    J_previous = J_current;
    NOI = NOI + 1;

end
end

