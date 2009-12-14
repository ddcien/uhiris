close all
clear
training = dlmread('my_training.txt');
labels = training(:,1);
training(:,1) = [];
num_of_samples = length(labels);

fid = fopen('my_libsvm_training.txt','w');
for i = 1:num_of_samples
    if labels(i)
        class = 1;
    else
        class = 0;
        if mod(i,3) ~= 0
            continue;
        end
    end
    fprintf(fid, '%d', class);
    for j = 1:12
        fprintf(fid, ' %d:%f', j,training(i,j));
    end
    fprintf(fid, '\n');
end
fclose(fid);
