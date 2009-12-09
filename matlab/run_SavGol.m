close all
clear

% a00 a10 a01 a20 a11 a02 a30 a21 a12 a03
A00 = 1; A10 = 2; A01 = 3; A20 = 4; A11 = 5; A02 = 6; A30 = 7; A21 = 8; A12 = 9; A03 = 10;
[NamesCoefs, NamesTerms, XPow, YPow, SG] = SavGol(3,5);

mov = aviread('sample2.avi');
num_of_frames = length(mov);
frame = mov(10).cdata;

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

% obtain partial derivatives etc.
f10x = A(:,:,A10);
f01y = A(:,:,A01);
f11xy = A(:,:,A11);
f20x = A(:,:,A20);
f02y = A(:,:,A02);

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
        if (ev(1) == ev(2) && ev(1) ~= 0)
            newton = [-f10x/f20x, -f01y/f02y];
            newton = newton / norm(newton);
        else
            vec1 = v(:,1); vec2 = v(:,2);
            
        end        
    end
end
toc


