function [ o ] = toImage( i )  
   o = cell(1,size(i,1));
   for k = 1 : size(i,1)
        o{k} = transpose(reshape(i(k,:), 28, 28));
   end 
end

