function show_result(x_k, error, alpha, iter, method, x_0)
% show_result(x_k, error, alpha, iter, method, x_0)
% x_k: the iterative point at each iteration; error: the difference between
% the global minimization and the function value at each iteration
% alpha: the step length at each iteration; method: the used method;
% x_0: the starting point
    figure;
    subplot(1, 2, 1);
    plot(1:iter, error);
    title(['Minimize the Rosenbrock functino using ', method])
    ylabel('error');
    xlabel('Iterations');
    subplot(1, 2, 2);
    plot(1:iter, alpha)
    title(['x_0 = ', mat2str(x_0', 3), ', x^* = ', mat2str(x_k(iter,:)', 3)]);
    ylabel('alpha');
    xlabel('Iterations');   
end