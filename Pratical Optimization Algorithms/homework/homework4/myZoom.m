function alpha = myZoom(x, p, slope_0, alpha_lo, alpha_hi, f0, fx, c1, c2)
% the zoom function
%
% OUTPUT:
% alpha: the step length got by successively decreasing the size of the interval until an acceptable step length is identified.
%
% INPUTS:
% x: an n-d column vector(the current point)
% p: the step direction
% slope_0: gradient of function at x in direction p
% alpha_lo: the lower bound for alpha
% alpha_hi: the uppper bound for alpha
% f0: the function value at x
% fx: the function value at last point
% c1: the parameter in the Armijo condition
% c2: the parameter in the curvature condition

    while (1)
        alpha = alpha_lo + (alpha_hi - alpha_lo) / 2;
        xc = x + alpha * p;
        f = Rosenbrock(xc);
        
        if (f > f0 + c1 * alpha * slope_0 || f > fx)
            alpha_hi = alpha;
        else
            slope_c = dfunc(xc)' * p;
            if (abs(slope_c) <= -c2 * slope_0)
                return;
            end
            if (slope_c * (alpha_hi - alpha_lo) >= 0)
                alpha_hi = alpha_lo;
                alpha_lo = alpha;
                fx = f;
            end
        end
    end

end