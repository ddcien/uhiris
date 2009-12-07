function selected = SelectiveSurfacePlot(gray_image)
figure; imshow(gray_image, []); hold on;
[x y] = ginput(2);
rectangle('Position',[x(1),y(1),x(2)-x(1),y(2)-y(1)],'EdgeColor','r'); hold off;
selected = gray_image(y(1):y(2),x(1):x(2));

figure; surf(selected);
