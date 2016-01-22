function [ D, E, F, G ] = purityOfSOMClusters( C, GT )    
    for i = 1:size(C,2)
        members = find(C(:,i) == 1);
        classes = GT(members);
        classGuess = mode(classes);
        sz = length(classes);
        purity = length(classes(classes==classGuess))/sz;
        
        D(i) = classGuess;
        E(i) = purity * 100; 
        F(i) = sz;
    end
    
    for i = 0:9
        G(i+1) = sum(E(D==(i)) .* (F(D ==(i)) ./ sum(F(D ==(i)))));
    end
    disp( sprintf( '&%2.1f&%2.1f&%2.1f&%2.1f&%2.1f&%2.1f&%2.1f&%2.1f&%2.1f&%2.1f&%2.1f', G, mean(G) ));


end

