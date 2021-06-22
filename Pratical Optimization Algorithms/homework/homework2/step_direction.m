function p = step_direction(x_k, method)
% p = step_direction(x_k, method)
% x_k: a 2-d column vector; method: a string ('steepest descend' or 'newton methd')
% This function returns the descending direction using steepest descend or
% newton method
    if strcmp(method, 'steepest descend')
        p = - Rb_gradient(x_k);
    else
        p = - Rb_hessian(x_k)^(-1) * Rb_gradient(x_k);
    
    p = p / norm(p);    %unitization the direction p
end