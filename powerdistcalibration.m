dist = [.63 .945 1.26 1.575 1.89 2.205 2.52 2.835 3.15 3.465];
lsb = [800 725 540 330 250 260 230 290 270 240];

dBm = (lsb-205)/37-26;
power = 10.^(dBm/10);

x0 = linspace(.63, 3.465, 100);
theory = [power(1)];
for i=2:100
    theory(i)=theory(i-1)*x0(i-1)^2/x0(i)^2;
end

delta = .007684777519; %calculated in excel
upper = theory + delta;
lower = theory - delta;

figure (1)
plot(dist, power, 'k*');
hold on;
plot(x0, theory, '-k');
hold on;
plot(x0, upper, '-r');
hold on;
plot(x0, lower, '-r');
title('Power vs Distance calibration curve');
xlabel('Distance (m)');
ylabel('Power (mW)');
legend('Measured Data', 'Theoretical Relationship', 'Theoretical Relationship + Uncertainty', 'Theoretical Relationship - Uncertainty');

