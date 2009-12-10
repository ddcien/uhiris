close all
clear
global A A00 A10 A01 A20 A11 A02 A30 A21 A12 A03
% a00 a10 a01 a20 a11 a02 a30 a21 a12 a03
A00 = 1; A10 = 2; A01 = 3; A20 = 4; A11 = 5; A02 = 6; A30 = 7; A21 = 8; A12 = 9; A03 = 10;
[NamesCoefs, NamesTerms, XPow, YPow, SG] = SavGol(3,5)

mov = aviread('sample2.avi');
num_of_frames = length(mov);
frame = mov(10).cdata;
% frame = imread('leye.jpg');

myeps = 0.0000001;

tic
if size(frame, 3) == 3
    img = double(rgb2gray(frame));
else
    img = double(frame);
end
[rows cols] = size(img);

g = fspecial('gaussian', 15, 2.5);
img = imfilter(img, g, 'symmetric');
% convolve the image with the computed set of filters to obtain the
% polynomial coefficients
A = zeros(rows, cols, 10);
num_of_coef = size(SG, 3);
for i = 1:num_of_coef
    A(:,:,i) = imfilter(img, SG(:,:,i), 'symmetric');
end
A(A<myeps) = 0;

%%%%%
coef = squeeze(A(88,132,:));
syms x y
symb = [1 x x^2 y x*y y^2];
fun = coef' * symb';
ezsurf(fun,[-2 2 -2 2]); hold on;
eyeimg = img(88+[-2:2], 132+[-2:2]);
surf(-2:2,-2:2,eyeimg); hold off;
figure;
%%%%%

% obtain partial derivatives etc.
f10x = A(:,:,A10);
f01y = A(:,:,A01);
f11xy = A(:,:,A11);
f20x = A(:,:,A20);
f02y = A(:,:,A02);

imshow(frame,[]); hold on;

% preallocation some stuff
hessian = zeros(2);
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
        if (abs(ev(1) - ev(2)) < eps && ev(1) ~= 0)
            newton = [-f10x(i,j)/f20x(i,j); -f01y(i,j)/f02y(i,j)];
            directional_vector = newton;
        else
            directional_vector = v;
            vec1 = v(:,1); vec2 = v(:,2);
        end
        zc = [];
        num_of_dv = size(directional_vector, 2);
        for k = 1:num_of_dv
            zc = [zc; SolveZeroCrossing(directional_vector(:,k),A(i,j,A10),A(i,j,A01),A(i,j,A20),A(i,j,A11),A(i,j,A02),A(i,j,A30),A(i,j,A21),A(i,j,A12),A(i,j,A03))];
        end
        if ~(isempty(zc))
            label = TopographicClassification(i, j, zc);
        else
            mag = norm([f10x(i,j) f01y(i,j)]);
            if mag == 0 && ev(1) > 0 && ev(2) > 0
                % pit
                plot(j,i,'r+');
            elseif mag < 10^-5 && abs(ev(1)) < 10^-5 && abs(ev(2)) < 10^-5
                % flat
                plot(j,i,'bo');
            end
        end
    end
end
hold off;
toc


