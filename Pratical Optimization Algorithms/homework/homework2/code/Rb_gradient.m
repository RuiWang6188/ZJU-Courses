function g = Rb_gradient(x)
% g = Rb_gradient(x)
% x : 2-d column vector
% This function calculates the gradient of Rosenbrock function at point x
    g = [-400*x(1)*(x(2)-x(1)^2)-2*(1-x(1));200*(x(2)-x(1)^2)];
end