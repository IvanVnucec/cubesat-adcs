function [euler] = qib2Euler(qib)

% Funkcija qib2Euler() racuna Eulerove kutove iz zadanog kvaterniona.
% Ulazni parametri:
%       qib = zadani kvaternion (4D stupcasti vektor)
% Izlazni parametri:
%       euler = izracunati Eulerovi kutovi (3D stupcasti vektor)

psi = atan2(2*(qib(2)*qib(3)+qib(1)*qib(4)), qib(1)^2+qib(2)^2-qib(3)^2-qib(4)^2);
theta = asin(-2*(qib(2)*qib(4)-qib(1)*qib(3)));
phi = atan2(2*(qib(3)*qib(4)+qib(1)*qib(2)), qib(1)^2-qib(2)^2-qib(3)^2+qib(4)^2);

psi = rad2deg(psi);
theta = rad2deg(theta);
phi = rad2deg(phi);

euler = [psi, theta, phi]';
end

