function [ y ] = toInd( x )
    if (size(x,1) ~= 1) 
        disp('expected a row vector');
        return;
    end
    v = unique(x);
    c = length(x);
    r = length(v);
    y = zeros(r, c);
    
    for i=1:r
        y(i,:) = (x == v(i)) ;
    end
end

