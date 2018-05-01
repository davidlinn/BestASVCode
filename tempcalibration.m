voltage = [1.73 1.88 1.94 2.04]; %percent of max fan speed
%REQUIRES ATTENTION
temp = [9.8 12.2 15.3 21.3]; %read in the white box
%REQUIRES ATTENTION
xmeas = voltage; %copying variable names for copy pasta purposes
ymeas = temp;

%coefficients of the fit and information about the errors
%REQUIRES ATTENTION
order = 1; %order of the polynomial fit
[coefficients, S] = polyfit(xmeas, ymeas, order); 

%values to plot the best fit curve over
%REQUIRES ATTENTION
xstart = 1.7; %start and end values to cover the range
%REQUIRES ATTENTION
xend = 2.1; %of the measured data
x0 = linspace(xstart,xend,100); 

%bestfit is the y values of the best fit, delta is something about the errors
[bestfit, delta] = polyval(coefficients, x0, S);

%uncertainties on the best fit line
upper = bestfit + delta;
lower = bestfit - delta;



lowtemp = 5;
hightemp = 25;
temps = linspace(lowtemp, hightemp, 100);
voltages = [];
R1 = 150000;
for i=1:100
    volt = 3.3*R1/(R1+ThermTempToRes(temps(i)));
    voltages(i) = volt;
end

[coefficients2, S2] = polyfit(voltages, temps, order);
[bestfit2, delta2] = polyval(coefficients2, x0, S);

%plot the original data and the best fit line with its uncertainties
plot(x0,bestfit2,'-b');
hold on;
plot(xmeas,ymeas,'*k');
hold on;
plot(x0, bestfit,'-k');
hold on;
plot(x0, upper, '-r');
hold on;
plot(x0, lower, '-r');
%REQUIRES ATTENTION
xlabel('Voltage (V)');
%REQUIRES ATTENTION
ylabel('Temperature (C)');
title('Temperature vs Voltage calibration curve');
legend('Theoretical Calibration Curve','Measured Data','Best Fit','Best Fit + Uncertainty','Best Fit - Uncertainty');
%REQUIRES ATTENTION
text(1.75, 24, ['Theoretical: Temperature = ' num2str(coefficients2(1)) ' * Voltage + ' num2str(coefficients2(2))]);
text(1.9, 10, ['Measured: Temperature = ' num2str(coefficients(1)) ' * Voltage + ' num2str(coefficients(2))]);

