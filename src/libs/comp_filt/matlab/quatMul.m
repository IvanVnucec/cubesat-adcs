function [q] = quatMul(q1,q2)

% Funkcija quatMul() racuna umnozak dva kvaterniona q1*q2.
% Ulazni parametri:
%       q1 = prvi zadani kvaternion (4D stupcasti vektor)
%       q2 = drugi zadani kvaternion (4D stupcasti vektor)
% Izlazni parametri:
%       q = umnozak zadanih kvaterniona q1*q2 (4D stupcasti vektor)

% Priprema matrice za matricno mnozenje
A = [q1(1), -q1(2), -q1(3), -q1(4); 
     q1(2),  q1(1), -q1(4),  q1(3);
     q1(3),  q1(4),  q1(1), -q1(2);
     q1(4), -q1(3),  q1(2),  q1(1)];

% Izracun umnoska q1*q2 u matricnom obliku
q = A*q2;

end

