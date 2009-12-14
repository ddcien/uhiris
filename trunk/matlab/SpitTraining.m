function SpitTraining(labels, eye)
eye = CleanUp(eye);
fid = fopen('training.txt','a');
num_of_candidates = size(eye,1);
examined = zeros(num_of_candidates);
patch_img = figure;
label_img = figure; imshow(labels,[]); hold on
[h w] = size(labels);
for i = 1:num_of_candidates
    this = eye(i, :);
    figure(label_img); plot(this(2),this(1),'ro');
    examined(i,i) = 1;
    for j = 1:num_of_candidates
        if examined(i,j) 
            continue; 
        end
        examined(i,j) = 1; examined(j,i) = 1; 
        target = eye(j, :);
        
        d = sqrt((this(1)-target(1))^2+(this(2)-target(2))^2);
        % arguable thresholds for different face scales
        if d < 120 && d > 40
            theta = atan2(this(1)-target(1), this(2)-target(2));
            rect_w = round(0.6 * d);
            rect_h = round(0.3 * d);
            rect = zeros(rect_h, rect_w);
            num_of_pixels = rect_h * rect_w;
            %%% current patch
            for r = 1:rect_h
                for c = 1:rect_w
                    old_c = (c-rect_w/2) * cos(theta) - (r-rect_h/2) *sin(theta);
                    old_r = (r-rect_h/2) * cos(theta) + (c-rect_w/2) *sin(theta);
                    old_r = old_r + this(1);
                    old_c = old_c + this(2);
                    if round(old_r) <= h && round(old_r) >= 1 && round(old_c) <= w && round(old_c) >= 1
                        rect(r,c) = labels(round(old_r),round(old_c));
%                       if (r==1&&c==1) || (r==rect_h&&c==rect_w) || (r==1&&c==rect_w) || (r==rect_h&&c==1)
%                          plot(old_c, old_r, 'g*');
%                       end
                    end                 
                end
            end
%             mu = mean(rect(:)); sig = std(rect(:));
            feat = hist(rect(:), 1:12) / num_of_pixels;
            figure(patch_img); imshow(rect, []);
            res = [];
            while (~strcmp(res,'l') && ~strcmp(res,'r') && ~strcmp(res,'n'))
                res = input('Is it an eye? L/R/N: ', 's');
                res = lower(res);
            end
            switch res
                case 'l'
                    class = 1;               
                case 'r'
                    class = 2;
                case 'n'
                    class = 0;
            end
            fprintf(fid, '%d ', class);
            for k = 1:12
                fprintf(fid, '%f ', feat(k));
            end
            fprintf(fid, '\n');
            %%% target patch
            clear rect; rect = zeros(rect_h, rect_w);
            figure(label_img); plot(target(2),target(1),'bo');
            for r = 1:rect_h
                for c = 1:rect_w
                    old_c = (c-rect_w/2) * cos(theta) - (r-rect_h/2) *sin(theta);
                    old_r = (r-rect_h/2) * cos(theta) + (c-rect_w/2) *sin(theta);
                    old_r = old_r + target(1);
                    old_c = old_c + target(2);
                    if round(old_r) <= h && round(old_r) >= 1 && round(old_c) <= w && round(old_c) >= 1
                        rect(r,c) = labels(round(old_r),round(old_c));
%                       if (r==1&&c==1) || (r==rect_h&&c==rect_w) || (r==1&&c==rect_w) || (r==rect_h&&c==1)
%                           plot(old_c, old_r, 'y*');
%                       end 
                    end                
                end
            end
%             mu = mean(rect(:)); sig = std(rect(:));
            feat = hist(rect(:), 1:12) / num_of_pixels;
            figure(patch_img); imshow(rect, []);
            res = [];
            while (~strcmp(res,'l') && ~strcmp(res,'r') && ~strcmp(res,'n'))
                res = input('Is it an eye? L/R/N: ', 's');
                res = lower(res);
            end
            switch res
                case 'l'
                    class = 1;               
                case 'r'
                    class = 2;
                case 'n'
                    class = 0;
            end
            fprintf(fid, '%d ', class);
            for k = 1:12
                fprintf(fid, '%f ', feat(k));
            end
            fprintf(fid, '\n');
            
            clear rect;
            figure(label_img); hold off; imshow(labels,[]); hold on;
            plot(this(2),this(1),'ro');
        end        
    end
    figure(label_img); hold off; imshow(labels,[]); hold on
end
fclose(fid);
close(label_img);
close(patch_img);