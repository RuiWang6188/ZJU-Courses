function G=d2func(x)
% It's Rosenbrock's Hessian.
% x: an n-d column vector.
    N = size(x, 1);
    if (N == 1)
        G = 0.0;
    else
        G = zeros(N);
        G(1, 1) = 1200.0 * x(1)^2 - 400.0 * x(2) + 2.0;
        G(1, 2) = -400.0 * x(1);
        for i = 2:N - 1
            G(i, i - 1) = -400.0 * x(i - 1);
            G(i, i + 1) = -400.0 * x(i);
            G(i, i) = 1200.0 * x(i)^2 - 400.0 * x(i + 1) + 202.0;
        end
        G(N, N - 1) = -400.0 * x(N - 1);
        G(N, N) = 200.0;
    end
end