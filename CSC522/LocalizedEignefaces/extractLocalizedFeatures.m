%%From the in, extact "Test" sets of "eigens" feateures, with a min subsegments size
%% of minSize by minSize. 
function [ Rs, Us ] = extractLocalizedFeatures(in, tests, minSize, eigens )
    imSz = 28; 
    b = imSz - minSize;
    
    Us = cell(tests,1);
    Rs = cell(tests,1);
    for i=1:tests
        disp(num2str(i));
        x = randperm(b,1);
        y = randperm(b,1);
        xS= randperm(imSz - max([x y]) - minSize + 1, 1) + minSize - 1;
      

        xT = getImageSegments(in, [x,y],[xS,xS]);
        %%Update to handle rectangels!!!!
        [Us{i},Rs{i}] = getEigenFaces(xT, eigens);
    end
end

