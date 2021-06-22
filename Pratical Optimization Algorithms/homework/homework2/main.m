function main()
    alpha_orig = 1;
    rho = 0.5;
    c = 1e-4;
    
    x_0 = [1.2; 1.2]; % the starting point
    [x_k, error, alpha, iter] = backtracking_line_search(x_0, 'steepest descend', alpha_orig, rho, c);
    [x_k, error, alpha, iter] = backtracking_line_search(x_0, 'newton method', alpha_orig, rho, c);
    
    x_1 = [1.2; 1]; % a more difficult starting point
    [x_k, error, alpha, iter] = backtracking_line_search(x_1, 'steepest descend', alpha_orig, rho, c);
    [x_k, error, alpha, iter] = backtracking_line_search(x_1, 'newton method', alpha_orig, rho, c)

end