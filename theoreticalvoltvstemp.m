lowtemp = 5;
hightemp = 25;
temps = linspace(lowtemp, hightemp, 100);
voltages = [];
R1 = 150000;
for i=1:100
    volt = 3.3*R1/(R1+ThermTempToRes(temps(i)));
    voltages(i) = volt;
end
    
plot(voltages,temps);

%1.198,5
%1.98,25