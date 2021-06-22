function [x_d, f_d, error, steps] = Steepest_descend(x, MAX_IT, tol)
    
    error = 1.0;
    steps = 0;
    convergent = 1;
    alpha = 1;
    x_d = x;
    rho = 0.7;
    c = 1e-4;
    
    while (error > tol) 
        f_d = Rosenbrock(x_d);
        g = dfunc(x_d);
        p = -g;
       
        alpha = LineSearch(x_d, p, 'Steepest_descend');
 
        x_d = x_d + alpha * p;
        steps = steps + 1;
        error = norm(g);
        
        if (steps > MAX_IT)
            convergent = 0;
            steps = -1;          
            break;
        end

    end

end 