function [ x ] = normalizeImages( x )
%NORMALIZEIMAGES Normalize a set of images. 
    x = x / max(max(x));
    avg = mean(x,1);
    x = bsxfun(@minus, x, avg);
end

