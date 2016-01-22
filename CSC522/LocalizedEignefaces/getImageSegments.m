function [ out ] = getImageSegments( in, st_c, sz_c )
%from a SAMPLES x PIXELS matrix takes an image subset defined by the start
%cord and the size cord. Returns a matrix SAMPLES x [sizeX * sizeY]. For
%conveinence in this particualr assignment the input matirx images size is
%always assumes to be square. 

    s_x= size(in,1);
    s_i= size(in,2)^.5;
    out = zeros(s_x, sz_c(1)*sz_c(2));
    
    p = 0; 
    for i = 1:s_x; 
        
        %if(mod(i, (s_x/100)) == 0) 
        %    p = p + 1;
        %    disp(num2str(p));
        %end
        
        im = reshape(in(i,:),s_i,s_i);
        x1 = st_c(1);
        x2 = (st_c(1)+sz_c(1) - 1);
        y1 = st_c(2);
        y2 = (st_c(2)+sz_c(2) - 1);
        subset = im(x1:x2,y1:y2);
        out(i,:) = reshape(subset, 1, sz_c(1)*sz_c(2));
        
    end
end

