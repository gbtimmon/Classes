[~,trainX, trainY, testX, testY] = loadData();

%make column centric;
t = trainX';

%bound to 0 to 1; 
t = t / max(max(t));
figure(1)
sampleFilters(t)
title('orig Images')

%find the avg image;
avg = mean(t,2);
figure(3)
imagesc(reshape(avg,28,28))
title('Avg Image')
%subtract the mean image from all images. 
tNorm = t - repmat(avg,1,size(t,2));
figure(2)
sampleFilters(tNorm)
title('Normalized Images')
%Cov 
cov = tNorm * tNorm';
figure(4)
imagesc(cov)
title('covarience mat')
%eig

figure(5)
[U,V,D] = svd(cov);
sampleFilters(U(:,1:204),0,1);
title('eigenfaces')

tRot = (U' * tNorm);

tRot0 = tRot;
tRot0(2:end,:) = 0;

tWhite = (tRot0' * inv(U))';
figure(6);
sampleFilters(tWhite)
