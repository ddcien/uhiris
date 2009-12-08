function labels = ProcessFrame(frame)
global h20 h10 h01 h11 h02

if size(frame, 3) == 3
    img = double(rgb2gray(frame));
else
    img = double(frame);
end

g = fspecial('gaussian', 15, 2.5);
img = imfilter(img, g, 'symmetric');
img = imfilter(img, g, 'symmetric');
f20xy = imfilter(img,h20,'symmetric');
f10x = imfilter(img,h10,'symmetric');
f01y = imfilter(img,h01,'symmetric');
f11xy = imfilter(img,h11,'symmetric');
f02xy = imfilter(img,h02,'symmetric');

imshow(frame); hold on;
hessian = zeros(2);
[rows cols] = size(img);
labels = zeros(rows, cols);
mag = zeros(rows, cols);
ev1 = zeros(rows, cols);
ev2 = zeros(rows, cols);
foo1 = zeros(rows, cols);
foo2 = zeros(rows, cols);
for i = 1:rows
    for j = 1:cols
        hessian(1,1) = f20xy(i,j);
        hessian(1,2) = f11xy(i,j);
        hessian(2,1) = f11xy(i,j);
        hessian(2,2) = f02xy(i,j);
        [v d] = eig(hessian);
        ev = diag(d);
        ev1(i,j) = ev(1); ev2(i,j) = ev(2);
        mag(i,j) = sqrt(f10x(i,j)^2+f01y(i,j)^2);
        if mag(i,j) < 1.4 && ev(1) > 1 && ev(2) > 1
            % pit
            plot(j,i,'r+');
        elseif mag(i,j) < 1.4 && ev(1) < -1 && ev(2) < -1
            % peak
            plot(j,i,'go');
        elseif mag(i,j) < 1.4 && ev(1) * ev(2) < -1
            % saddle
            plot(j,i,'bx');
        elseif (mag(i,j) > 1.4 && ev(1) < -1 && abs([f10x(i,j) f01y(i,j)] * v(:,1)) < 0.01) || ...
            (mag(i,j) > 1.4 && ev(2) < -1 && abs([f10x(i,j) f01y(i,j)] * v(:,2)) < 0.01) || ...
            (mag(i,j) < 1.4 && ev(1) < -1 && abs(ev(2)) < 0.01)
            % ridge
            plot(j,i,'y+');
        elseif (mag(i,j) > 1.4 && ev(1) > 1 && abs([f10x(i,j) f01y(i,j)] * v(:,1)) < 0.01) || ...
            (mag(i,j) > 1.4 && ev(2) > 1 && abs([f10x(i,j) f01y(i,j)] * v(:,2)) < 0.01) || ...
            (mag(i,j) < 1.4 && ev(1) > 1 && abs(ev(2)) < 0.01)
            % ravine
            plot(j,i,'co');
        elseif (mag(i,j) < 1.4 && abs(ev(1)) < 0.1 && abs(ev(2)) < 0.1)
            % flat
            plot(j,i,'mx');
        end
        foo1(i,j) = [f10x(i,j) f01y(i,j)] * v(:,1);
        foo2(i,j) = [f10x(i,j) f01y(i,j)] * v(:,2);
    end
end
hold off
% figure; imshow(mag,[]);
% figure; imshow(ev1,[]);
% figure; imshow(ev2,[]);
