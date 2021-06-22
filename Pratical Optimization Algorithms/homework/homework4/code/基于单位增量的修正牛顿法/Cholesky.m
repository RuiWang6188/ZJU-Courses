function L = Cholesky(M)
% M: n*n spd. matrix
% L: the Cholesky factorization matrix

    n = length(M);
    L = zeros(n, n);
    for i = 1 : n
        if (M(i, i) - L(i, :) * L(i, :)') < 0
            L = zeros(n, n);
            break;
        else
            L(i, i) = sqrt(M(i, i) - L(i, :) * L(i, :)');
        end
        for j = (i + 1) : n
            L(j, i) = (M(j, i) - L(i, :) * L(j, :)') / L(i, i);
        end
    end
end