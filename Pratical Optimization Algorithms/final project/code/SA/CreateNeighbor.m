function route2 = CreateNeighbor( route1 )

	pSwap=0.4;
	pReversion=0.4;
	pInsertion = 1-pSwap-pReversion;


	% Roulette Wheel Selection
    randNum = rand;
    p = cumsum([pSwap pReversion pInsertion]); 
    M = find(randNum <= p, 1, 'first');



	switch M
	    case 1
	        route2=Swap(route1);       
	    case 2
	        route2 = Reversion(route1);     
	    case 3
	        route2 = Insertion(route1);      
	end


end


function res = Swap(x)

	N = numel(x);

	i=randsample(N,2);
	i1=i(1);
	i2=i(2);

	res=x;

	res(i1)=x(i2);
	res(i2)=x(i1);

end

function res = Reversion(x)

	N = numel(x);

	i=randsample(N,2);
	i1=min(i);
	i2=max(i);

	res=x;
	res(i1:i2)=x(i2:-1:i1);

end

function res = Insertion(x)

	N = numel(x);

	i=randsample(N,2);
	i1=i(1);
	i2=i(2);

	if i1<i2
	    res=x([1:i1-1 i1+1:i2 i1 i2+1:end]); 
	else
	    res=x([1:i2 i1 i2+1:i1-1 i1+1:end]);    
	end

end
