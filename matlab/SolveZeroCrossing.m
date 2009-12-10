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

C1 = 2*a20*sin(theta)^2 + a11*sin(theta)*cos(theta) + 2*a02*cos(theta)^2;
C2 = a10*sin(theta) + a01*cos(theta);
C = [C1 C2 C3];

zc = [];
rho = roots(C);
num_of_roots = size(rho, 1);
if (isreal(rho) && ~isempty(rho))
    realroots = [rho * sin(theta) rho * cos(theta)];
    for i = 1:num_of_roots
        if abs(realroots(i,:)) <= 0.5
            zc = [zc; realroots(i,1) realroots(i,2)];
        end
    end
end
