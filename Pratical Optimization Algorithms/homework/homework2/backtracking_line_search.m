function [x_k, error, alpha, iter] = backtracking_line_search(x_0, method, alpha_orig, rho, c)
% [x_k, error, alpha, iter] = backtracking_line_search(x_0, method, alpha_orig, rho, c)
% x_k: the iterative point at each iteration; error: the difference between 
%the global minimization and the function value at each iteration
% alpha: the step length at each iteration; iter: the iteration times
% x_0: the starting point; method: the used method(steepest descend or newton); 
% alpha_orig: the original step length; rho: a scale factor; c: c1 in the Armijo condition

    iter = 1;
    tol = 1e-6;
    max_iter = 1e6;
    error = zeros(max_iter, 1);
    f_k = zeros(max_iter, 1);
    x_k = zeros(max_iter, 2);
    alpha = alpha_orig * ones(max_iter, 1);
    x_k(1,1) = x_0(1);
    x_k(1,2) = x_0(2);
    alpha(1) = 0;
    f_k(1) = Rosenbrock(x_0);
    
    x = x_0;
    while iter < max_iter && norm(Rb_gradient(x)) > tol
        iter = iter + 1;
        p = step_direction(x, method);
        [alpha(iter), f_k(iter)] = step_length(x, p, alpha(iter), rho, c);
        x = x + alpha(iter) * p;
        x_k(iter, 1) = x(1);
        x_k(iter, 2) = x(2);
    end
    error = abs(0 - f_k);
    x_k = x_k(1:iter, :);
    f_k = f_k(1:iter);
    alpha = alpha(1:iter);
    error = error(1:iter);
end