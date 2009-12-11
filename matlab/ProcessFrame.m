function [labels eye] = ProcessFrame(frame)
global h20 h10 h01 h11 h02
UNKNOWN = 0; 
PEAK = 1; PIT = 2; 
RIDGESADDLE = 3; RAVINESADDLE = 4; 
RIDGE = 5; RAVINE = 6; FLAT = 7; 
CONVEXHILL = 8; CONCAVEHILL = 9;
CONCAVESADDLEHILL = 10; CONVEXSADDLEHILL = 11; SLOPEHILL = 12;
Tmag = 1.4; Tev = 1; Tge = 0.01;

if size(frame, 3) == 3
    img = double(rgb2gray(frame));
else
    img = double(frame);
end

g = fspecial('gaussian', 17, 3);
img = imfilter(img, g, 'symmetric');
img = imfilter(img, g, 'symmetric');
f20x = imfilter(img,h20,'symmetric');
f10x = imfilter(img,h10,'symmetric');
f01y = imfilter(img,h01,'symmetric');
f11xy = imfilter(img,h11,'symmetric');
f02y = imfilter(img,h02,'symmetric');

imshow(frame); hold on;
hessian = zeros(2);
[rows cols] = size(img);
labels = zeros(rows, cols);
mag = sqrt(f10x.^2+f01y.^2);
eye = [];
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
            eye = [eye; i j];
            labels(i,j) = PIT;
        elseif mag(i,j) < Tmag && ev(1) * ev(2) < -Tev
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
            elseif (ev(1) > Tev && ev(2) >= Tev) || (ev(1) >= Tev && ev(2) > Tev)
                labels(i,j) = CONVEXHILL;
            elseif (ev(1) < -Tev && ev(2) <= -Tev) || (ev(1) <= -Tev && ev(2) < -Tev)
                labels(i,j) = CONCAVEHILL;
            elseif ev(1) * ev(2) < -Tev
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
