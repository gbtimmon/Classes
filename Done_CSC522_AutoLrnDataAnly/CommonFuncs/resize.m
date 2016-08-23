function [ newMat ] = resize( images, xS, yS, wn )
imgCt  = size(images,3);
xSize  = xS;
ySize  = yS;
window = wn;

if( mod(xSize, window) ~= 0 || mod(ySize, window) ~= 0)
    warning('window does not evenly divide image')
end
xIter = (xSize / window);
yIter = (ySize / window);
newMat = zeros( imgCt, xIter * yIter);
for picInd = 1:imgCt %start with a few
    newPic = zeros(1, xIter * yIter);
    for j = 1:xIter
        for k = 1:yIter
            x0 = ( ( k - 1 ) * window ) + 1;
            x1 = x0 + (window - 1);
            y0 = ( ( j - 1 ) * window ) + 1;
            y1 = y0 + (window - 1);
            sg = images(x0:x1,y0:y1, picInd);
            cm = sum(sum(sg));
            in =((k - 1) * xIter) + (j - 1) + 1 ;
            newPic( in ) = cm ;
        end
    end
    newMat(picInd, :) = newPic;
end

