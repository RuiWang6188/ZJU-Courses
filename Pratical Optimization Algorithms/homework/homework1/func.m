function func(A)
% func(A)
% A: a 2-d symmetric matrix
    x = -10:0.1:10;
    y = -10:0.1:10;
    [X, Y] = meshgrid(x,y);
    Z = zeros(201,201);
    
    for i = 1:201
        for j = 1:201
            Z(i,j) = 1/2 .*[X(i, j), Y(i, j)] * A * [X(i, j), Y(i, j)]';
        end
    end
    
    subplot(1,2,1);
    contour(X, Y, Z);
    subplot(1,2,2);
    surf(X, Y, Z);
    
end