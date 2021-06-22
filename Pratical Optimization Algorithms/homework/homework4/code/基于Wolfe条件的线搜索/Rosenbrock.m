function f = Rosenbrock(x)
% It's Rosenbrock.
% x: an n-d column vector.func
    f = sum(100.0 * (x(2:end) - x(1:end - 1).^2).^2 + (1 - x(1:end - 1)).^2);
end