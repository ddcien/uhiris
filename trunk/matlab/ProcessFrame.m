function [labels eyes] = ProcessFrame(frame)
global h20 h10 h01 h11 h02

myeps = 10^-6;

%%% Topographic labels
UNKNOWN = 0; 
PEAK = 1; PIT = 2; RIDGE = 3; RAVINE = 4;
RIDGESADDLE = 5; RAVINESADDLE = 6;
CONVEXHILL = 7; CONCAVEHILL = 8;
CONVEXSADDLEHILL = 9; CONCAVESADDLEHILL = 10;
SLOPEHILL = 11; FLAT = 12; 

%%% Empirical thresholds
Tmag = 1.4; Tev = 1; Tge = 0;

%%% Polynomial coefficient indices
% a00 + a10 * x + a20 * x^2 + a01 * y + a11 * x * y + a02 * y^2
A00 = 1; A10 = 2; A20 = 3; A01 = 4; A11 = 5; A02 = 6;

if size(frame, 3) == 3
    img = double(rgb2gray(frame));
else
    img = double(frame);
end
[rows cols] = size(img);

g = fspecial('gaussian', 15, 2.5);
img = imfilter(img, g, 'symmetric');
img = imfilter(img, g, 'symmetric');
f10x = imfilter(img,h10,'symmetric'); f10x(abs(f10x)<myeps)=0;
f20x = imfilter(img,h20,'symmetric'); f20x(abs(f20x)<myeps)=0;
f01y = imfilter(img,h01,'symmetric'); f01y(abs(f01y)<myeps)=0;
f11xy = imfilter(img,h11,'symmetric'); f11xy(abs(f11xy)<myeps)=0;
f02y = imfilter(img,h02,'symmetric'); f02y(abs(f02y)<myeps)=0;

%%%%%
% A = zeros(rows, cols, 6);
% A(:,:,A10) = f10x;
% A(:,:,A20) = f20x / 2;
% A(:,:,A01) = f01y;
% A(:,:,A11) = f11xy;
% A(:,:,A02) = f02y / 2;
% loc = [195, 408];
% coef = squeeze(A(loc(1),loc(2),:));
% syms x y
% symb = [1 x x^2 y x*y y^2];
% fun = coef' * symb';
% ezsurf(fun,[-2 2 -2 2]);
% eyeimg = img(loc(1)+[-2:2],loc(2)+[-2:2]);
% figure; surf(-2:2,-2:2,eyeimg);
% figure;
%%%%%

imshow(frame); hold on;
hessian = zeros(2);
labels = zeros(rows, cols);
mag = sqrt(f10x.^2+f01y.^2);
eyes = [];
for i = 1:rows
    for j = 1:cols
        hessian(1,1) = f20x(i,j);
        hessian(1,2) = f11xy(i,j);
        hessian(2,1) = f11xy(i,j);
        hessian(2,2) = f02y(i,j);
        [v d] = eig(hessian);
        ev = diag(d);

        if mag(i,j) < Tmag && ev(1) < -Tev && ev(2) < -Tev
            % peak
%             plot(j,i,'go');
            labels(i,j) = PEAK;
        elseif mag(i,j) < Tmag && ev(1) > Tev && ev(2) > Tev
            % pit
            plot(j,i,'r+');
            eyes = [eyes; i j];
            labels(i,j) = PIT;
        elseif mag(i,j) < Tmag && ev(1) * ev(2) < 0
            % saddle
%             plot(j,i,'bx');
            if sum(ev) < 0
                labels(i,j) = RIDGESADDLE;
            else
                labels(i,j) = RAVINESADDLE;
            end
        elseif (mag(i,j) >= Tmag && ev(1) < -Tev && abs([f10x(i,j) f01y(i,j)] * v(:,1)) < Tge) || ...
            (mag(i,j) >= Tmag && ev(2) < -Tev && abs([f10x(i,j) f01y(i,j)] * v(:,2)) < Tge) || ...
            (mag(i,j) < Tmag && ev(1) < -Tev && abs(ev(2)) <= Tev) || ...
            (mag(i,j) < Tmag && abs(ev(1)) <= Tev && ev(2) < -Tev)
            % ridge
%             plot(j,i,'y*');
            labels(i,j) = RIDGE;
        elseif (mag(i,j) >= Tmag && ev(1) > Tev && abs([f10x(i,j) f01y(i,j)] * v(:,1)) < Tge) || ...
            (mag(i,j) >= Tmag && ev(2) > Tev && abs([f10x(i,j) f01y(i,j)] * v(:,2)) < Tge) || ...
            (mag(i,j) < Tmag && ev(1) > Tev && abs(ev(2)) <= Tev) || ...
            (mag(i,j) < Tmag && abs(ev(1)) <= Tev && ev(2) > Tev)
            % ravine
%             plot(j,i,'co');
            labels(i,j) = RAVINE;
        elseif (mag(i,j) < Tmag && abs(ev(1)) <= Tev && abs(ev(2)) <= Tev)
            % flat
%             plot(j,i,'rx');
            labels(i,j) = FLAT;
        elseif (abs([f10x(i,j) f01y(i,j)] * v(:,1)) >= Tge && abs([f10x(i,j) f01y(i,j)] * v(:,2)) >= Tge) || ...
                (abs([f10x(i,j) f01y(i,j)] * v(:,1)) >= Tge && abs(ev(2)) <= Tev) || ...
                (abs([f10x(i,j) f01y(i,j)] * v(:,2)) >= Tge && abs(ev(1)) <= Tev) || ...
                (mag(i,j) >= Tmag && abs(ev(1)) <= Tev && abs(ev(2)) <= Tev)
            % hillside
%             plot(j,i,'mx');
            if abs(ev(1)) <= Tev && abs(ev(2)) <= Tev
                labels(i,j) = SLOPEHILL;
            elseif (ev(1) > 0 && ev(2) >= 0) || (ev(1) >= 0 && ev(2) > 0)
                labels(i,j) = CONVEXHILL;
            elseif (ev(1) < 0 && ev(2) <= 0) || (ev(1) <= 0 && ev(2) < 0)
                labels(i,j) = CONCAVEHILL;
            elseif ev(1) * ev(2) < 0
                if sum(ev) < 0
                    labels(i,j) = CONVEXSADDLEHILL;
                else
                    labels(i,j) = CONCAVESADDLEHILL;
                end
            end
        else
            labels(i,j)=UNKNOWN;
        end
    end
end
hold off
