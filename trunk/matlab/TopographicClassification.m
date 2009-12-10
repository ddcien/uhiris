function label = TopographicClassification(x, y, zc)
global A A00 A10 A01 A20 A11 A02 A30 A21 A12 A03 
num_of_zc = size(zc, 1);
% if num_of_zc > 1
    f10x = A(x,y,A10)+2*A(x,y,A20)*zc(:,1)+A(x,y,A11)*zc(:,2)+3*A(x,y,A30)*zc(:,1).^2+2*A(x,y,A21)*(zc(:,1).*zc(:,2))+A(x,y,A12)*zc(:,2).^2;
    f01y = A(x,y,A01)+A(x,y,A11)*zc(:,1)+2*A(x,y,A02)*zc(:,2)+A(x,y,A21)*zc(:,1).^2+2*A(x,y,A12)*(zc(:,1).*zc(:,2))+3*A(x,y,A03)*zc(:,2).^2;
    mag = zeros(num_of_zc, 1);
    for i = 1:num_of_zc
        mag(i) = norm([f10x(i) f01y(i)]);
    end
    zc = zc(mag==min(mag),:);
    min_idx = find(mag==min(mag));
    mag = mag(min_idx(1));
% end

f20x = 2*A(x,y,A20)+6*A(x,y,A30)*zc(1)+2*A(x,y,A21)*zc(2);
f11xy = A(x,y,A11)+2*A(x,y,A21)*zc(1)+2*A(x,y,A12)*zc(2);
f02y = 2*A(x,y,A02)+2*A(x,y,A12)*zc(1)+6*A(x,y,A03)*zc(2);
hessian = zeros(2);
hessian(1,1) = f20x;
hessian(1,2) = f11xy;
hessian(2,1) = f11xy;
hessian(2,2) = f02y;

[v d] = eig(hessian);
ev = diag(d);

if mag == 0 && ev(1) > 0 && ev(2) > 0
    % pit
    plot(y,x,'r+');
    label = 1;
elseif mag < 10^-5 && abs(ev(1)) < 10^-5 && abs(ev(2)) < 10^-5
    % flat
    plot(y,x,'bo');
    label = 2;
else
    label = 0;
end



