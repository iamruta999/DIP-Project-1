%NAME: Amruta Kulkarni
%USC ID: 6914970827
%USC Email: arkulkar@usc.edu
%Submission date: 20 Sept 2015
%Problem 3C

% After the three color matrices are generated using C++, they are copied
% in the current folder. The individual matrices are then read using
% readraw followed by BM3D filtering.
red = readraw('redNoisyPeppers.raw');
green = readraw('greenNoisyPeppers.raw');
blue = readraw('blueNoisyPeppers.raw');
% Apply the function BM3D
[PSNRr, y_est_red] = BM3D(1,red,60,'np',1);
[PSNRg, y_est_green] = BM3D(1,green,60,'np',1);
[PSNRb, y_est_blue] = BM3D(1,blue,60,'np',1);
% Combine individual noise free 8 bit images to form a colored noisr free
% 24 bits image
ImageBM3D = zeros(512,512,3);
ImageBM3D(:,:,1) = y_est_red;
ImageBM3D(:,:,2) = y_est_green;
ImageBM3D(:,:,3) = y_est_blue;
imshow(ImageBM3D)

