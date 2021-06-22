function alpha = LineSearch(x, p, method)
% The line search algorithm based on strong Wolfe condition
% 
% OUTPUTS:
% alpha: the step length fits the strong Wolfe condition
%
% INPUTS:
% x: an n-d column vector(the current point)
% p: the step direction

    c1 = 1e-4;
    if strcmp(method, 'Newton')
        c2 = 1;
    else
        c2 = 0.1;
    
     alpha_MAX = 100;
     alpha = 1;
     alpha_0 = 0;
     alpha_1 = alpha;
     
     f0 = Rosenbrock(x);
     
     fx = f0;
     iter = 1;
     
     while 1
        
         xc = x + alpha_1 * p;
         f = Rosenbrock(xc);
        
         if ((f > f0 + c1 * alpha_1 * dfunc(x)' * p) || ((f >= fx)&& (iter > 1)))
            alpha = myZoom(x, p, dfunc(x)' * p, alpha_0, alpha_1, f0, fx, c1, c2);
            break;
         end
         
         if (abs(dfunc(xc)' * p) <= -c2 * dfunc(x)' * p )
             alpha = alpha_1;
             break;
         end
         
         if (dfunc(xc)' * p >= 0)
             alpha = myZoom(x, p, dfunc(x)' * p, alpha_1, alpha_0, f0, f, c1, c2);
             break;
         end
         
         alpha_0 = alpha_1;
         alpha_1 = min(alpha_MAX, alpha_1 * 2);
         fx = f;   
     end
end