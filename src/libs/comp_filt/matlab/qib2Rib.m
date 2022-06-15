function [Rib] = qib2Rib(qib)

% Funkcija qib2Rib() racuna rotacijsku matricu iz zadanog kvaterniona.
% Ulazni parametri:
%       qib = zadani kvaternion (4D stupcasti vektor)
% Izlazni parametri:
%       Rib = izracunata rotacijska matrica (3x3)

% Izracun elemenata rotacijske matrice iz kvaterniona
r11 = qib(1)^2+qib(2)^2-qib(3)^2-qib(4)^2;
r12 = 2*(qib(2)*qib(3)+qib(1)*qib(4));
r13 = 2*(qib(2)*qib(4)-qib(1)*qib(3));

r21 = 2*(qib(2)*qib(3)-qib(1)*qib(4));
r22 = qib(1)^2-qib(2)^2+qib(3)^2-qib(4)^2;
r23 = 2*(qib(3)*qib(4)+qib(1)*qib(2));

r31 = 2*(qib(2)*qib(4)+qib(1)*qib(3));
r32 = 2*(qib(3)*qib(4)-qib(1)*qib(2));
r33 = qib(1)^2-qib(2)^2-qib(3)^2+qib(4)^2;

% Kreiranje rotacijske matrice I -> B od izracunatih elemenata
Rib = [r11,r12,r13;
       r21,r22,r23;
       r31,r32,r33];
end

