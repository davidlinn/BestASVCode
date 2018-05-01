function output = ThermTempToRes(temp)
tempK = temp + 273.15;
A = -16.0349;
B = 5459.339;
C = -191141;
D = -3328322;
Rref = 100000;

output=Rref*exp(A+B/tempK+C/tempK^2+D/tempK^3);
end

