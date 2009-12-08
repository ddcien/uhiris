%%
close all
clear
global h20 h10 h01 h11 h02

h20 = sgsdf_2d2(-2:2,-2:2,2,2,2,0,1);
h10 = sgsdf_2d2(-2:2,-2:2,2,2,1,0,1);
h01 = sgsdf_2d2(-2:2,-2:2,2,2,0,1,1);
h11 = sgsdf_2d2(-2:2,-2:2,2,2,1,1,1);
h02 = sgsdf_2d2(-2:2,-2:2,2,2,0,2,1);

% rgb = imread('smallleye.jpg');
mov = aviread('sample2.avi');
num_of_frames = length(mov);
% M(num_of_frames) = struct('cdata',[],'colormap',[]);

for k = 15:15
    rgb = mov(k).cdata;
    labels = ProcessFrame(rgb);
end
% movie(M);     