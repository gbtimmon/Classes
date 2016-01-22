[~,x,y,x2,y2] = loadData();
Config.extractions        = [0,1,2,3];
Config.useGlobal          = true;
Config.features           = 5;
Config.repetitions        = 5; 
Config.minsize            = 10;
Config.Opts.UseParralel   = true;
Config.maxIter            = 500;
Config.clusters           = 20;
Config.kmeansDisp         = 'iter';

totalMat = zeros(length(Config.extractions),11);
for r = 1:Config.repetitions %repeat many times. 
    testRes = zeros(length(Config.extractions),11);
    iters = max(length(Config.extractions),length(Config.features));
    
    for i = 1:iters;
        %find the number of local extracts
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %t = Config.extractions(1);
        t = Config.extractions(i);
 
        %Find the number of features
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        e = Config.features(1);          %constant eigens
        %e = Config.features(i);         %dynamic eigens
        %e = round(e / (t));              %to bound by total features.
        e = round(e / (t+1));           
        
        %FIND THE MINSIZE
        %%%%%%%%%%%%%%%%%
        minsize = ceil(e^.5);
        %minsize = Config.minsize(1);
        %minsize = Config.minsize(i);
        
        if(Config.useGlobal)
            M = zeros(e*(t+1), 60000);
            [~,M(1:e,:)] =  getEigenFaces(x,e);
            
            if ( t > 0 ) 
                [R,~] = extractLocalizedFeatures(x, t, minsize, e);
                for j = 1:length(R)
                    M(((j)*e+1):(j+1)*e,:) = R{j};
                end
            end
        else
            M = zeros(e*(t), 60000);
            if ( t > 0 )
                [R,~] = extractLocalizedFeatures(x, t, minsize, e);
                for j = 1:length(R)
                    M(((j-1)*e+1):(j)*e,:) = R{j};
                end
            else
                error('No Test run');
            end
        end
        
        clusters = kmeans(M',Config.clusters,...
                          'Options', Config.Opts,...
                          'MaxIter', Config.maxIter,...
                          'Display', Config.kmeansDisp);
                      
        [~,~,~,D] = purityOfSOMClusters(toInd(clusters')',y');
        testRes(i,:) = [D, mean(D)];
    end
     totalMat = totalMat + testRes;
end
totMat = totalMat / Config.repetitions ;