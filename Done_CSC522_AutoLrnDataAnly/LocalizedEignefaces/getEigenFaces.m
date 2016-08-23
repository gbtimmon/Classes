function [ U, xRot ] = getEigenFaces( x, k )
    x = normalizeImages(x); 
    c = cov(x); 
    [U,V] = svd(c);
    U = U(:,1:k);
    xRot = U' * x'; 
end

