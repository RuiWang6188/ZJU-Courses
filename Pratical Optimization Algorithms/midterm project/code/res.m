N = [500	1000	2000	3000	4000	5000	6000	7000	8000	9000	10000	20000	30000	40000	50000	15000	25000	35000	45000];
time = [1.79E-07	2.18E-07	2.45E-07	2.94E-07	2.98E-07	3.05E-07	3.24E-07	3.24E-07	3.46E-07	3.52E-07	3.91E-07	4.75E-07	5.20E-07	5.33E-07	5.60E-07	4.29E-07	4.80E-07	5.33E-07	5.47E-07];
scatter(N, time,'g');
hold on
%plot(N, time, 'g')
hold on
title('Time for N Search');
xlabel N
ylabel time/s

plot(xdata,ydata,'ko')
hold on
tlist = linspace(xdata(1),xdata(end));
plot(tlist,X(5) * (1/(1+exp(X(1)))).^((1/(1+exp(X(2)))).^((1-(1/(1+exp(X(3)))).^tlist).^(1+exp(X(4))))),'b-')
xlabel PO2
ylabel SO2
title('Levenberg-Marquardt Method Fit to Data')
legend('Data','Fit Curve')
hold off