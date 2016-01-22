function [ ] = toTable( A )
for i = 1:size(A,1)
    disp( sprintf('&%2.1f&%2.1f&%2.1f&%2.1f&%2.1f&%2.1f&%2.1f&%2.1f&%2.1f&%2.1f&\\underline{%2.1f}\\\\\n',A(i,:)));
end

end

