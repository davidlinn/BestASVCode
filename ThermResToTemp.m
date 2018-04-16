function output = ThermResToTemp(Res)
C3 = 0.003354016; %A1
C4 = 0.0002460382; %B1
C5 = 0.000003405377; %C1
C6 = 0.000000103424; %D1
C8 = 100000; %R_ref

C7 = Res
tempK=(C3+C4.*log(C7./C8)+C5.*log(C7./C8).*log(C7./C8)+C6.*log(C7./C8).*log(C7./C8).*log(C7./C8)).^(-1);
output=tempK-273.15;
end