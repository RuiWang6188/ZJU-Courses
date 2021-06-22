function resultStruct = SA(position)

    % Parameter
    MaxIter = 30000;     %Maximum Iteration
    N = 50;    %Maximum Iteration For Each Temperature
    T0 = 200;         %Initial Temperature
    alpha = 0.93;     %Temperature Reduction Rate


    temp = position';

    pos = [temp(:,1)'; temp(:,2)']';
    distMatrix = pdist2(pos,pos);

    cityStruct.X = temp(:,1)';
    cityStruct.Y = temp(:,2)';
    cityStruct.N = numel(temp(:,1)');
    cityStruct.distMatrix = distMatrix;

    CostFunction = @(tour) TourLenth(tour,cityStruct);


    % Initialation

    n = cityStruct.N;
    sol.Position = randperm(n);

    sol.Cost = CostFunction(sol.Position);          %Create initial Solution

    optSolution = sol;         %Initialize Best solution
    distHistory=zeros(MaxIter,1);    %Initialize Array To Hold Best Cost

    T = T0;     %Initialize Temperature


    % Main Loop
    for it = 1 : MaxIter


        for subIt = 1 : N
            newSol.Position = CreateNeighbor(sol.Position); %Create New Solution
            newSol.Cost = CostFunction(newSol.Position);

            if newSol.Cost <= sol.Cost % new solution better
                sol = newSol;

            else
                DELTA = (newSol.Cost - sol.Cost)/sol.Cost;
                P = exp(-DELTA/T);
                if rand <= P
                    sol = newSol;
                end
            end


            if sol.Cost <= optSolution.Cost  %Update Best Solotion
                optSolution = sol;
            end

        end

        distHistory(it) = optSolution.Cost; %Update Best Cost

        T = alpha*T;    %Update Temperature
        disp(['Iteration ' num2str(it) ' Temperature = ' num2str(T) ' And Best Cost Found = ' num2str(distHistory(it))]);
    end


    %print result
    figure;
    plot(distHistory,'LineWidth',2);
    xlabel('Iteration');
    ylabel('minDist');
    title('Best Solution History');


    figure;
    x = cityStruct.X;
    y = cityStruct.Y;
    BestSolotion = [optSolution.Position optSolution.Position(1)];
    plot(x(BestSolotion),y(BestSolotion),'r.-','MarkerSize',10,'MarkerFaceColor','y','LineWidth',2);
    title(sprintf('Total Distance = %1.4f',distHistory(MaxIter)));


    resultStruct = struct(...
        'position', position,...
        'optSolution', optSolution,...
        'minDist', distHistory(MaxIter),...
        'distHistory', distHistory,...
        'Temperature', T);

end



function len = TourLenth( tour , cityStruct )
    n = numel(tour);
    tour = [tour  tour (1)];
    len = 0;
    for k=1 : n
        i = tour(k);
        j = tour (k+1);
        
        len = len + cityStruct.distMatrix(i,j);
    end
end
