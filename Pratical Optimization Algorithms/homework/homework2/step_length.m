function [alpha, f_k] = step_length(x_k, p_k, alpha_orig, rho, c)
%[alpha, f_k] = step_length(x_k, p_k, alpha_orig, rho, c)
% x_k: a 2-d column vector; p_k: the chosen descending direction
% alpha_orig: the original step length; rho: a scale factor; c: c1 in the Armijo condition
% This function returns the step length according to the Armijo condition

    alpha = alpha_orig;
    f_k = Rosenbrock(x_k);
    while(Rosenbrock(x_k + alpha * p_k) > Rosenbrock(x_k) + c * alpha * Rb_gradient(x_k)' * p_k)
        alpha = rho * alpha;
    end
    
end