function h=sgsdf_2d2(x,y,nx,ny,dx,dy,flag_coupling)
% 2-D Savitzky-Golay (SG) Smoothing and Differentiation Filter. 
% The filter coefficients are calculated from the decomposation property of the 2-D SG filters. 
% In addition, the filter coefficients for mixed partial derivative can be obtained by this function. 
%
%   h=sgsdf_2d2(x,y,nx,ny,d,flag_coupling)
%       x    = x data point, e.g., -3:3 (both odd-length and even-lenght
%       are accepted, symmetry is prefered but not restricted)
%       y    = y data point, e.g., -2:2   (both odd-length and even-lenght
%       are accepted, symmetry is prefered but not restricted)
%       nx    =x polynomial order       default=1
%       ny    =y polynomial order       default=1
%      dx    = differentiation order in x direction (0<=d<=nx)    default=0
%      dy    = differentiation order in y direction (0<=d<=ny)    default=0
%       flag_coupling  = with or without the consideration of the coupling
%                                   terms, between x and y. default=0
%       h   = filter coefficients obtained.
%
% Example:
%        sgsdf_2d2(-2:2,-3:3,1,1,0,0,0)
%        sgsdf_2d2(-2:2,-3:3,1,1,0,0,1)
%        sgsdf_2d2(-2:2,-3:3,2,2,0,1,0)
%        sgsdf_2d2(-2:2,-3:3,2,2,0,1,1)
%        sgsdf_2d2((-3:2)+1/2,-3:3,2,2,1,0,0)
%        sgsdf_2d2((-3:2)+1/2,-3:3,2,2,1,0,1)
%        sgsdf_2d2(-1:1,-1:1,1,1,1,1,1)
%        sgsdf_2d2((-3:2)+1/2,(-4:3)+1/2,2,2,1,1,1)
%        sgsdf_2d2(-3:3,-4:4,2,3,2,1,1)
%
% Author:
%     Jianwen Luo <luojw@ieee.org>
%     11/23/2005
%
% References:
% [1]	J. W. Luo, K. Ying, P. He, and J. Bai, "Properties of Savitzky-Golay Digital Differentiators,"
% Digital Signal Processing, vol. 15, pp. 122-136, 2005.
% [2]	A. Savitzky and M. J. E. Golay, "Smoothing and Differentiation of Data by Simplified Least Squares Procedures,"
% Analytical Chemistry, vol. 36, pp. 1627-1639, 1964.
% [3]	K. L. Ratzlaff and J. T. Johnson, "Computation of Two-Dimensional Polynomial Least-Squares Convolution Smoothing Integers,"
% Analytical Chemistry, vol. 61, pp. 1303-1305, 1989.
% [4]	J. E. Kuo, H. Wang, and S. Pickup, "Multidimensional Least-Squares Smoothing Using Orthogonal Polynomials,"
% Analytical Chemistry, vol. 63, pp. 630-635, 1991.
% [5] http://research.microsoft.com/users/jckrumm/SavGol/SavGol.htm
% [6]	T. A. Sakharuk, "Computation of Weighting Functions for Smoothing 2-Dimensional Data by Local Polynomial-Approximation Techniques,"
% Analytica Chimica Acta, vol. 249, pp. 331-336, 1991.
% [7]	P. Meer and I. Weiss, "Smoothed Differentiation Filters for Images,"
% Journal of Visual Communication and Image Representation, vol. 3, pp. 58-72, 1992.
% [8]	P. Nikitas and A. Pappa-Louisi, "Comments on the two-dimensional smoothing of data,"
% Analytica Chimica Acta, vol. 415, pp. 117-125, 2000.

if nargin<7
    flag_coupling=0;
end

if nargin<6
    dy=0;
end

if nargin<5
    dx=0;
end

if nargin<4
    ny=0;
end

if nargin<3
    nx=0;
end

h1=sgsdf(x,nx,dx);%length(x)
h2=sgsdf(y,ny,dy);%length(y)
h=zeros(length(y),length(x));

if flag_coupling==0
    
    if dx==0 && dy==0
        for k1=1:length(y)
            for k2=1:length(x)
                h(k1,k2)=h1(k2)/length(y)+h2(k1)/length(x)-1/length(x)/length(y);
            end
        end
        
    elseif dx==0        
        for k1=1:length(y)
            for k2=1:length(x)
                h(k1,k2)=h2(k1)/length(x);
            end     
        end
        
    elseif dy==0
        for k1=1:length(y)
            for k2=1:length(x)
                h(k1,k2)=h1(k2)/length(y);
            end
        end
        
    else
        h=zeros(length(y),length(x));
    end
    
else 
    for k1=1:length(y)
        for k2=1:length(x)
            h(k1,k2)=h1(k2)*h2(k1);
        end
    end
end

function fc=sgsdf(x,n,dn,x0,flag)
%Function:
%       Savitzky-Golay Smoothing and Differentiation Filter
%       The Savitzky-Golay smoothing/differentiation filter (i.e., the polynomial smoothing/differentiation filter,
%       or  the least-squares smoothing/differentiation filters) optimally fit a set of data points to polynomials
%       of different degrees.
%       See for detail in Matlab Documents (help sgolay).  The sgolay function in Matlab can deal with only
%       symmetrical and uniformly spaced data of even number.
%       This function presented here is a general implement of the sgolay function in Matlab. The Savitzky-Golay filter
%       coefficients for even number, nonsymmetrical and nonuniformly spaced data can be obtained. And the filter coefficients
%       for the initial point or the end point can be obtained too.In addition, either numerical results or symbolical
%       results can be obtained.
%Usage:
%       fc   = sgsdf(x,n,dn,x0,flag)
%       x    = the original data point, e.g., -5:5
%       n    = polynomial order,                    default=1
%       dn   = differentation order (0=smoothing),  default=0
%       x0   = estimation point,                    default=0
%       flag = numerical(0) or symbolical(1),       default=0
%       fc   = filter coefficients obtained.
%Notes:
%1.     x  can be arbitrary, e.g., odd number or even number,symmetrical or nonsymmetrical, uniformly spaced
%          or nonuniformly spaced, etc.
%2.     x0 can be arbitrary, e.g., the initial point,the end point,etc.
%3.     Either numerical results or symbolical results can be obtained.
%Example:
%       sgsdf([-3:3],2,0,0,0)
%       sgsdf([-3:3],2,0,0,1)
%       sgsdf([-3:3],2,0,-3,1)
%       sgsdf([-3:3],2,1,2,1)
%       sgsdf([-2:3],2,1,1/2,1)
%       sgsdf([-5:2:5],2,1,0,1)
%       sgsdf([-1:1 2:2:8],2,0,0,1)
%Author:
%       Jianwen Luo <luojw@bme.tsinghua.edu.cn, luojw@ieee.org> 2003-10-05
%       Department of Biomedical Engineering, Department of Electrical Engineering
%       Tsinghua University, Beijing 100084, P. R. China
%Reference
%[1]A. Savitzky and M. J. E. Golay, "Smoothing and Differentiation of Data by Simplified Least Squares Procedures,"
%   Analytical Chemistry, vol. 36, pp. 1627-1639, 1964.
%[2]J. Steinier, Y. Termonia, and J. Deltour, "Comments on Smoothing and Differentiation of Data by Simplified Least Square Procedures,"
%   Analytical Chemistry, vol. 44, pp. 1906-1909, 1972.
%[3]H. H. Madden, "Comments on Savitzky-Golay Convolution Method for Least-Squares Fit Smoothing and Differentiation of Digital Data,"
%   Analytical Chemistry, vol. 50, pp. 1383-1386, 1978.
%[4]R. A. Leach, C. A. Carter, and J. M. Harris, "Least-Squares Polynomial Filters for Initial Point and Slope Estimation,"
%   Analytical Chemistry, vol. 56, pp. 2304-2307, 1984.
%[5]P. A. Baedecker, "Comments on Least-Square Polynomial Filters for Initial Point and Slope Estimation,"
%   Analytical Chemistry, vol. 57, pp. 1477-1479, 1985.
%[6]P. A. Gorry, "General Least-Squares Smoothing and Differentiation by the Convolution (Savitzky-Golay) Method,"
%   Analytical Chemistry, vol. 62, pp. 570-573, 1990.
%See also:
%       sgolay

if nargin<5
    flag=0;
end
if nargin<4
    x0=0;
end
if nargin<3
    dn=0;
end
if nargin<2
    n=1;
end


if(n>length(x)-1)
    error('Polynomial Order too Large');
end
if(dn>n)
    error('Differentiation Order too Large!');
end

if flag
    x=sym(x);
    x0=sym(x0);
end

x=x(:);
A=ones(length(x),1);
for k=1:n
    A=[A x.^k];
end
h=inv(A'*A)*A';

if flag
    hx=sym([(zeros(1,dn)) prod(1:dn)]);  %order=0:dn-1,& dn,respectively
    for k=1:n-dn                        %order=dn+1:n=dn+k
        hx=[hx sym(x0^k*prod(dn+k:-1:k+1))];
    end
else
    hx=[(zeros(1,dn)) prod(1:dn)];  %order=0:dn-1,& dn,respectively
    for k=1:n-dn                        %order=dn+1:n=dn+k
        hx=[hx x0^k*prod(dn+k:-1:k+1)];
    end
end
% h
% hx
hx=repmat(hx',[1,size(h,2)]);
h=h.*hx;
if size(h,1)>1%n=0
    fc=sum(h);
else
    fc=h;
end
