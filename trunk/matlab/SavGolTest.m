function [r] = SavGolTest(SG)
% Test Savitzky-Golay coefficients
% John Krumm, Microsoft Research, August 2001
% Extract size of filter and number of terms
[nSize,nDum,nTerms] = size(SG);

% Compute order of polynomial based on nTerms = (nOrder+1)(nOrder+2)/2
nOrder = -3/2 + sqrt(9/4 - 2*(1-nTerms));

% Make terms of polynomial
syms x y real;
Terms = [];
for j=0:nOrder
    for i=0:nOrder-j
        Terms = [Terms; (x^i)*(y^j)];
    end
end

% Make an image patch using this polynomial with all coefficients being one (1)
true_coef = [1 5 -1 2 0 0];
Image = [];
for y = -(nSize-1)/2:(nSize-1)/2 % important to loop through in same scan order as image patch pixels
    for x = -(nSize-1)/2:(nSize-1)/2
        Image = [Image; subs(true_coef * Terms)];
    end
end
Image = reshape(Image,nSize,nSize);

% Apply filters, all results (coefficients) should be one (1)
Coefs = [];
for i=1:nTerms
    Coefs = [Coefs; sum(sum(SG(:,:,i).*Image))];
end

surf(Image); hold on;
ezsurf(Coefs'*Terms, ((nSize-1)/2) * [-1 1 -1 1]); hold on;
% ezsurf(true_coef*Terms, ((nSize-1)/2) * [-1 1 -1 1]);

% Compare, need round() to compensate for roundoff error
if any( (Coefs' - true_coef) > 10^-5 )
    r = 0; % failed test
else
    r = 1; % passes test
end

 
