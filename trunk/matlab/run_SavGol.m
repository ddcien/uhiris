%%
close all
clear

% a00 a10 a01 a20 a11 a02 a30 a21 a12 a03
[NamesCoefs, NamesTerms, XPow, YPow, SG] = SavGol(3,5);

mov = aviread('sample2.avi');
num_of_frames = length(mov);
frame = mov(10).cdata;


if size(frame, 3) == 3
    img = double(rgb2gray(frame));
else
    img = double(frame);
end

g = fspecial('gaussian', 15, 2.5);
img = imfilter(img, g, 'symmetric');