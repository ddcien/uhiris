close all
clear
global A A10 A20 A01 A11 A02 myeps
% a00 + a10 * x + a20 * x^2 + a01 * y + a11 * x * y + a02 * y^2
A00 = 1; A10 = 2; A20 = 3; A01 = 4; A11 = 5; A02 = 6;

myeps = 10^-6;

[NamesCoefs, NamesTerms, XPow, YPow, SG] = SavGol(2,5);
% SG = zeros(5,5,6);
% SG(:,:,2) = sgsdf_2d2(-2:2,-2:2,2,2,1,0,1);
% SG(:,:,3) = sgsdf_2d2(-2:2,-2:2,2,2,2,0,1);
% SG(:,:,4) = sgsdf_2d2(-2:2,-2:2,2,2,0,1,1);
% SG(:,:,5) = sgsdf_2d2(-2:2,-2:2,2,2,1,1,1);
% SG(:,:,6) = sgsdf_2d2(-2:2,-2:2,2,2,0,2,1);
% 
% SG(:,:,A10) = dlmread('h10.txt')';
% SG(:,:,A20) = dlmread('h20.txt')';
% SG(:,:,A01) = dlmread('h01.txt')';
% SG(:,:,A11) = dlmread('h11.txt')';
% SG(:,:,A02) = dlmread('h02.txt')';


mov = aviread('sample2.avi');
num_of_frames = length(mov);
frame = mov(10).cdata;
% frame = imread('leye.jpg');

tic
if size(frame, 3) == 3
    img = double(rgb2gray(frame));
else
    img = double(frame);
end
[rows cols] = size(img);

g = fspecial('gaussian', 17, 3);
img = imfilter(img, g, 'symmetric');
img = imfilter(img, g, 'symmetric');
% convolve the image with the computed set of filters to obtain the
% polynomial coefficients
num_of_coef = size(SG, 3);
A = zeros(rows, cols, num_of_coef);
for i = 2:num_of_coef
    % REMOVE the transpose if not computed from SavGol
    A(:,:,i) = imfilter(img, SG(:,:,i)', 'symmetric');
end
% A(:,:,A20) = A(:,:,A20)/2;
% A(:,:,A02) = A(:,:,A02)/2;
% A(A<myeps) = 0;

%%%%%
coef = squeeze(A(88,133,:));
syms x y
symb = [1 x x^2 y x*y y^2];
fun = coef' * symb';
ezsurf(fun,[-2 2 -2 2]); hold on;
eyeimg = img(88+[-2:2], 133+[-2:2]);
figure; surf(-2:2,-2:2,eyeimg); hold off;
figure;
%%%%%

% obtain partial derivatives etc.
f10x = A(:,:,A10);
f01y = A(:,:,A01);
f11xy = A(:,:,A11);
f20x = A(:,:,A20)*2;
f02y = A(:,:,A02)*2;

imshow(frame,[]); hold on;

% preallocation some stuff
hessian = zeros(2);
mag = sqrt(f10x.^2+f01y.^2);
ev1 = zeros(rows, cols);
ev2 = zeros(rows, cols);
for i = 1:rows
    for j = 1:cols
        hessian(1,1) = f20x(i,j);
        hessian(1,2) = f11xy(i,j);
        hessian(2,1) = f11xy(i,j);
        hessian(2,2) = f02y(i,j);
        [v d] = eig(hessian);
        ev = diag(d);
        
        % search in the direction of the eigenvectors for a zero-crossing
        % of the first directional derivative within the pixel's area, if
        % the eigenvalues are equal and nonzero, search in the Newton
        % direction
        if (abs(ev(1) - ev(2)) < myeps && ev(1) ~= 0)
            newton = [-f10x(i,j)/f20x(i,j); -f01y(i,j)/f02y(i,j)];
            directional_vector = newton;
        else
            directional_vector = v;
            vec1 = v(:,1); vec2 = v(:,2);
        end
        zc = [];
        num_of_dv = size(directional_vector, 2);
        for k = 1:num_of_dv
            zc = [zc; SolveZeroCrossing(directional_vector(:,k),i,j)];
        end
        if ~(isempty(zc))
            [m eval evec] = TopographicClassification(i, j, zc);
            mag(i,j) = m;
            ev = eval;
            v = evec;
        end
        
        ev1(i,j) = ev(1); ev2(i,j) = ev(2);
        if mag(i,j) == 0 && ev(1) > 0 && ev(2) > 0
            % pit
            plot(j,i,'r+');
        elseif mag(i,j) == 0 && ev(1) < 0 && ev(2) < 0
            % peak
            plot(j,i,'yx');
        elseif mag(i,j) == 0 && ev(1) == 0 && ev(2) == 0
            % flat
            plot(j,i,'bo');
        end
        
    end
end
hold off;
toc


