%%
close all
clear
global h20 h10 h01 h11 h02
h10 = sgsdf_2d2(-2:2,-2:2,2,2,1,0,1);
h20 = sgsdf_2d2(-2:2,-2:2,2,2,2,0,1);
h01 = sgsdf_2d2(-2:2,-2:2,2,2,0,1,1);
h11 = sgsdf_2d2(-2:2,-2:2,2,2,1,1,1);
h02 = sgsdf_2d2(-2:2,-2:2,2,2,0,2,1);

% rgb = imread('smallleye.jpg');
% mov = aviread('out_frame_big2.avi');
% num_of_frames = length(mov);
% M(num_of_frames) = struct('cdata',[],'colormap',[]);

images = dir('data/*.png');
num_of_images = length(images);

for i = 1:num_of_images
    rgb = imread(fullfile('data',images(i).name));
    fprintf('Frame %d\n',i);
%     rgb = mov(i).cdata;
%      rgb = rgb(50:350,180:480,:);
    [labels eyes] = ProcessFrame(rgb);
%      figure; imshow(labels, []);
%     SpitTraining(labels, eyes);
%     M(i) = getframe;
    det = strcat(images(i).name(1:strfind(images(i).name,'.png')-1),'_det');
    saveas(gcf, det, 'png')
end
% movie(M);     