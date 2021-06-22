function g=dfunc(x)
% It's Rosenbrock's gradient.
    N = size(x, 1);
    if (N == 1)
        g = 0.0;
    else
        g = zeros(N, 1);
        g(1) = 2.0 * (x(1) - 1) + 400.0 * x(1) * (x(1)^2 - x(2));
        g(2:end - 1) = 2.0 * (x(2:end - 1) - 1.0)...
            + 400.0 * x(2:end - 1) .* (x(2:end - 1).^2 - x(3:end))... 
            + 200.0 * (x(2:end - 1) - x(1:end - 2).^2);
        g(end) = 200.0 * (x(end) - x(end - 1)^2);
    end
end