function r = genRow(x,y)
% for computing least square estimation
r = [1 x x^2 y x*y y^2];