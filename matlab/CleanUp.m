function clean = CleanUp(noisyarray)
ub = length(noisyarray);
examined = zeros(ub,ub);
mask = ones(ub, 1);
for i = 1:ub
    examined(i,i) = 1;
    for j = 1:ub
        if ~mask(j) || examined(i,j)
            continue;
        end
        examined(i,j) = 1; examined(j,i) = 1;
        dist = sqrt((noisyarray(i,1)-noisyarray(j,1))^2+(noisyarray(i,2)-noisyarray(j,2))^2);
        if dist < 3
            mask(j) = 0;
        end        
    end
end

clean = zeros(length(find(mask==1)),2);
k = 1;
for i = 1:ub
    if mask(i)
        clean(k, :) = noisyarray(i,:);
        k = k + 1;
    end
end


