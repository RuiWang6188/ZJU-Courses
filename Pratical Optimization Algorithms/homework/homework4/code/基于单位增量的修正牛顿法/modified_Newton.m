function [x_d, f_d, error, steps] = modified_Newton(x, MAX_ITER, tol)
% Newton method
% OUTPUTS:
% x_d: the search result;
% f_d: the value at the result point; 
%error: the difference between the search result and the minimizer;
% steps: the interation times.
% INPUTS:
% x: an n-d column vector;
% MAX_ITER: the maximum iteration; 
% tol: the allowed tolerance

    error = 1.0;
    steps = 0;
    convergent = 1;
    alpha = 1;
    x_d = x;
    beta = 1e-3;
    tau = 0;

    
    while (error > tol)
        f_d = Rosenbrock(x_d);
        g = dfunc(x_d);
        G = d2func(x_d);
       
 %       p = -G\g;
        min_aii = min(diag(G));
        n = length(G);
        L = zeros(n, n);
        
        if  min_aii > 0
            tau = 0;
        else tau = -min_aii + beta;
        end
        
        while 1
            L = Cholesky(G + tau * eye(n));
            if L == zeros(n)
                tau = max(2 * tau, beta);
            else
                break;
            end      
        end
        
        p = -(L * L')\g;
      %  p = - (G + tau * eye(n))\g;
        
        x_d = x_d + alpha * p;
        
        steps =  steps + 1;
        error = norm(g);
        
        if(steps > MAX_ITER)
            invergent = 0;
            steps = -1;
            break;
        end
          
    end
end