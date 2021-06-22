function h = Rb_hessian(x)
% h = Rb_hessian(x)
% x : a 2-d column vector
% This function calculates the Hessian matrix of the Rosenbrock function at
% point x
    h = [1200*x(1)^2-400*x(2)+2, -400*x(1);
        -400*x(1),               200      ];
end