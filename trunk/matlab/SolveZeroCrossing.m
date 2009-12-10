function zc = SolveZeroCrossing(vect,x,y)
% INPUT
%   vect    : directional vector
%   a**     : polynomial coefficients
% OUTPUT
%   x       : x of the zero-crossing, if within the pixel area -0.5:0.5
%   y       : y of the zero-crossing, if within the pixel area -0.5:0.5

global A A10 A20 A01 A11 A02
a10 = A(x,y,A10);
a20 = A(x,y,A20);
a01 = A(x,y,A01);
a11 = A(x,y,A11);
a02 = A(x,y,A02);

% the direction angle is the clockwise angle from the column (y) axis
theta = pi/2 - atan2(vect(2), vect(1));

% C1 * rho + C2 = 0
C1 = 2*a20*sin(theta)^2 + a11*sin(theta)*cos(theta) + 2*a02*cos(theta)^2;
C2 = a10*sin(theta) + a01*cos(theta);

zc = [];
if C1 ~= 0
    rho = -C2 / C1;
    x = rho * sin(theta);
    y = rho * cos(theta);
    if abs(x) <= 0.5 && abs(y) <= 0.5
        zc = [x y];
    end
end
