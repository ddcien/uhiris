function zc = SolveZeroCrossing(vect, a10, a01, a20, a11, a02, a30, a21, a12, a03)
% INPUT
%   vect    : directional vector
%   a**     : polynomial coefficients
% OUTPUT
%   x       : x of the zero-crossing, if within the pixel area -0.5:0.5
%   y       : y of the zero-crossing, if within the pixel area -0.5:0.5

% the direction angle is the clockwise angle from the column (y) axis
theta = pi/2 - atan2(vect(2), vect(1));

C1 = 3*a30*sin(theta)^3 + 3*a21*sin(theta)^2*cos(theta) + 3*a12*sin(theta)*cos(theta)^2+3*a03*cos(theta)^3;
C2 = 2*a20*sin(theta)^2 + 2*a11*sin(theta)*cos(theta) + 2*a02*cos(theta)^2;
C3 = a10*sin(theta) + a01*cos(theta);
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
