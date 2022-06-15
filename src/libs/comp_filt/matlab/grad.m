function [grad_J] = grad(a_i, m_i, e_a, e_m, qib)
% Funkcija q_gd() je iterativni estimator kvaterniona koja prima:   
%       a_i = mjereni vektor akceleracije u inercijalnom koordinatnom sustavu
%       m_i = mjereni vektor akceleracije u inercijalnom koordinatnom sustavu
%       e_a = vektor greške akcelerometra
%       e_m = vektor greške magnetometra
%       qib = zadani kvaternion u kojem raèunamo graijent
% Funkcija q_gd() vraca vektor koji sadrzi dva elementa:
%       grad_J = gradijent funkcije cilja za zadani kvaternion


% Inicijaliziram gradijent funkcije cilja za zadani kvaternion
grad_J = [0 0 0 0]';

% Parcijalna derivacija rotacijske matrice Rib_gd po q1
M1 = 2*[ qib(1)  qib(4) -qib(3);
        -qib(4)  qib(1)  qib(2);
         qib(3) -qib(2)  qib(1)];

% Parcijalna derivacija rotacijske matrice Rib_gd po q2
M2 = 2*[ qib(2)  qib(3)  qib(4);
         qib(3) -qib(2)  qib(1);
         qib(4) -qib(1) -qib(2)];

% Parcijalna derivacija rotacijske matrice Rib_gd po q3
M3 = 2*[-qib(3)  qib(2) -qib(1);
         qib(2)  qib(3)  qib(4);
         qib(1)  qib(4) -qib(3)];

% Parcijalna derivacija rotacijske matrice Rib_gd po q4
M4 = 2*[-qib(4)  qib(1)  qib(2);
        -qib(1) -qib(4)  qib(3);
         qib(2)  qib(3)  qib(4)];
     
% Raèunam gradijent za zadani kvaternion
grad_J(1) = 2*e_a'*M1*a_i  +  2*e_m'*M1*m_i;
grad_J(2) = 2*e_a'*M2*a_i  +  2*e_m'*M2*m_i;
grad_J(3) = 2*e_a'*M3*a_i  +  2*e_m'*M3*m_i;
grad_J(4) = 2*e_a'*M4*a_i  +  2*e_m'*M4*m_i;

end

