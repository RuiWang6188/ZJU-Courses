function f = Rosenbrock(x)
% f = Rosenbrock(x)
% x : 2-d column vector
% This function calculates the value of 2-d Rosenbrock function at point x
    f = 100 * (x(2)-x(1).^2).^2+(1-x(1)).^2;
end