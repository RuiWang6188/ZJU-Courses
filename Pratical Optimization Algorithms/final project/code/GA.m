function resultStruct = GA(position)
    position = position';
    distMatrix  = [];
    popSize     = 100;
    numIter     = 1e3;
    Pc          = 0.5;
    Pm          = 0.9;

    N = size(position,1);
    a = meshgrid(1:N);
    distMatrix = reshape(sqrt(sum((position(a,:)-position(a',:)).^2,2)),N,N);
    
    popSize     = 4*ceil(popSize/4);
    numIter     = max(1,round(real(numIter(1))));
    
    % Initialize the population
    pop = zeros(popSize,N);
    pop(1,:) = (1:N);
    for k = 2:popSize
        pop(k,:) = randperm(N);
    end

    globalMin = Inf;
    historyDist = zeros(1,numIter);
    tmpPop = zeros(4,N);
    newPop = zeros(popSize,N);

    figure('Name','TSP_ga | Current Best Solution','Numbertitle','off');
    hAx = gca;

    for iter = 1:numIter
        % Evaluate the length
        row = pop;
        col = pop(:,[2:N 1]);
        ind = N*(col-1) + row;
        totalDist = sum(distMatrix(ind),2);
              
        % Select the best
        [minDist,index] = min(totalDist);
        historyDist(iter) = minDist;
        if (minDist < globalMin)
            globalMin = minDist;
            optimalRoute = pop(index,:);
            % Plot the best route
            temp = optimalRoute([1:N 1]);
            plot(hAx,position(temp,1),position(temp,2),'r.-'); 
            title(hAx,sprintf('Total Distance = %1.4f, Iteration = %d',minDist,iter));
            drawnow;
        end
        
        randomOrder = randperm(popSize);
        for p = 4:4:popSize
            rtes = pop(randomOrder(p-3:p),:);
            dists = totalDist(randomOrder(p-3:p));
            if rand(1) < Pc % Use Cross Over and Mutation
                [ig,ord] = sort(dists); %#ok
                Parent1 = rtes(ord(1),:);
                Parent2 = rtes(ord(2),:);
                location = sort(ceil(N*rand(1,2)));
                i = location(1);
                j = location(2);
                len = j - i + 1;
                p1 = Parent1(i:j)';
                p2 = Parent2(i:j)';
     
                % Create Two Children From Two Best Parents
                child1 = Parent1;
                child1(any(p2(:,ones(N,1)) == Parent1(ones(len,1),:),1)) = [];
                child1 = [child1(1:i-1) Parent2(i:j) child1(i:N-len)];
                child2 = Parent2;
                child2(any(p1(:,ones(N,1)) == Parent2(ones(len,1),:),1)) = [];
                child2 = [child2(1:i-1) Parent1(i:j) child2(i:N-len)];
                
                if rand(1) < Pm
                    % Mutate child1
                    mutant = child1;
                    location = sort(ceil(N*rand(1,2)));
                    i = location(1);
                    j = location(2);
                    mutant(i:j) = mutant(j:-1:i);
                    newPop(p-3:p,:) = [Parent1; child1; child2; mutant];
                else
                    newPop(p-3:p,:) = [Parent1; child1; child2; Parent2];
                end
                elseif rand(1) < Pm % Use Mutation Only
                    [ig,idx] = min(dists); %#ok
                    bestOf4Route = rtes(idx,:);
                    location = sort(ceil(N*rand(1,2)));
                    i = location(1);
                    j = location(2);
                    for k = 1:4     % Mutate the best to get three new routes
                        tmpPop(k,:) = bestOf4Route;
                        switch k
                            case 2 % Flip
                                tmpPop(k,i:j) = tmpPop(k,j:-1:i);
                            case 3 % Swap
                                tmpPop(k,[i j]) = tmpPop(k,[j i]);
                            case 4 % Slide
                                tmpPop(k,i:j) = tmpPop(k,[i+1:j i]);
                            otherwise % Do nothing
                        end
                    end
                newPop(p-3:p,:) = tmpPop;
            else
                newPop(p-3:p,:) = pop(p-3:p,:);
            end

        end
        pop = newPop;    
    end
 
    % Format the optimal solution
    index = find(optimalRoute == 1,1);
    optimalSolution = [optimalRoute([index:N 1:index-1]) 1];
            
    % Plot the results
    figure('Name','TSP_ga | Results','Numbertitle','off');
    subplot(2,2,1);
    pclr = ~get(0,'DefaultAxesColor');
    plot(position(:,1),position(:,2),'.','Color',pclr); 
    title('City Locations');
    subplot(2,2,2);
    temp = optimalSolution;
     plot(position(temp,1),position(temp,2),'r.-'); 
    title(sprintf('Total Distance = %1.4f',minDist));
    subplot(2,2,3);
    plot(historyDist,'b','LineWidth',2);
    title('Best Solution History');
    set(gca,'XLim',[0 numIter+1],'YLim',[0 1.1*max([1 historyDist])]);
        

    % Save results in resultStruct
    resultStruct = struct( ...
        'position',          position, ...
        'distMatrix',        distMatrix, ...
        'popSize',     popSize, ...
        'numIter',     numIter, ...
        'optimalRoute',    optimalRoute, ...
        'optimalSolution', optimalSolution, ...
        'minDist',     minDist);    
end



