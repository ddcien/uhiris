close all
clear
training = dlmread('multipie_training.txt');
labels = training(:,1);
training(:,1) = [];
num_of_samples = length(labels);

num_of_negative = length(find(labels == 0));
num_of_positive = length(labels) - num_of_negative;
positive_samples = zeros(num_of_positive,12);
negative_samples = zeros(num_of_negative,12);
p = 1; n = 1;
for i = 1:num_of_samples
    if labels(i)
        positive_samples(p,:) = training(i,:);
        p = p + 1;
    else
        negative_samples(n,:) = training(i,:);
        n = n + 1;
    end    
end

dist = zeros(num_of_samples, 1);
sorted_samples = [positive_samples; negative_samples];
for i = 1:num_of_samples
    dist(i) = bhattacharyya(positive_samples(1,:),sorted_samples(i,:));
end
plot(1:num_of_samples,dist);