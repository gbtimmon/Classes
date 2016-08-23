function avg = avgImage(cells)
    %k = zeros(size(cells{1}));
    %for i = 1:length(cells)
    %    k = k + cells{i};
    %end
    %avg = k ./ length(cells);
    avg = mean(cells, 1) ./ max(max(cells));
end

