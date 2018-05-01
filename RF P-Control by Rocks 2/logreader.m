
clear;
clf;

filenum = '000';
infofile = strcat('INF', filenum, '.TXT');
datafile = strcat('LOG', filenum, '.BIN');

% map from datatype to length in bytes
dataSizes.('float') = 4;
dataSizes.('ulong') = 4;
dataSizes.('int') = 4;
dataSizes.('int32') = 4;
dataSizes.('uint8') = 1;
dataSizes.('uint16') = 2;
dataSizes.('char') = 1;





%% read from info file to get log file structure
fileID = fopen(infofile);
items = textscan(fileID,'%s','Delimiter',',','EndOfLine','\r\n');
fclose(fileID);
[ncols,~] = size(items{1});
ncols = ncols/2;
varNames = items{1}(1:ncols)';
varTypes = items{1}(ncols+1:end)';
varLengths = zeros(size(varTypes));
colLength = 256;
for i = 1:numel(varTypes)
    varLengths(i) = dataSizes.(varTypes{i});
end


R = cell(1,numel(varNames));

%% read column-by-column from datafile
fid = fopen(datafile,'rb');
for i=1:numel(varTypes)
    %# seek to the first field of the first record
    fseek(fid, sum(varLengths(1:i-1)), 'bof');
    
    %# % read column with specified format, skipping required number of bytes
    R{i} = fread(fid, Inf, ['*' varTypes{i}], colLength-varLengths(i));
    eval(strcat(varNames{i},'=','R{',num2str(i),'};'));
end

fclose(fid);
clf;
% figure (1)
% x = x(1:end);
% y = y(1:end);
% x = x(abs(x)<1000);
% y = y(abs(y)<1000);
% plot(x,y);
% for i = 1:length(x)
%      plot(x(1:i),y(1:i),'bo');
%      pause(0.001);
% end
% xlabel('x');
% ylabel('y');
% title('position');

samples=size(x);
% figure (2)
% plot(A17);
% title('unfiltered temp');
temp_unfiltered = cast(A17, 'double');
temp_unfiltered = temp_unfiltered(end-samples(1)+1:end);
% temp_mags = abs(fft(temp_unfiltered));
% num_bins = length(temp_mags);
[b, a] = butter(2, .01, 'low');
temp_filtered = filter(b,a,temp_unfiltered);
figure (2)
plot(temp_filtered);
title('filtered temp');

figure (6)
plot(A16);
title('unfiltered 90 deg');
turb_unfiltered = cast(A16, 'double');
turb_unfiltered = turb_unfiltered(end-samples(1)+1:end);
% turb_mags = abs(fft(turb_unfiltered));
% num_bins = length(turb_mags);
[b, a] = butter(2, .005, 'low');
turb_filtered = filter(b,a,turb_unfiltered);
figure (3)
plot(turb_filtered);
title('filtered 90 deg');

figure (5)
plot(A15);
title('unfiltered straight');
straight_unfiltered = cast(A15, 'double');
straight_unfiltered = straight_unfiltered(end-samples(1)+1:end);
% turb_mags = abs(fft(turb_unfiltered));
% num_bins = length(turb_mags);
[b, a] = butter(2, .005, 'low');
straight_filtered = filter(b,a,straight_unfiltered);
figure (4)
plot(straight_filtered);
title('filtered straight');

figure (7)
turbidity = straight_filtered./turb_filtered;
plot(turbidity);
title('turbidity');

xshore = 25;
yshore = 34;
distfromshore = ((x-xshore).^2+(y-yshore).^2).^(1/2);
start=4050;
ending=5200;


%REQUIRES ATTENTION
orderforce = 1; %order of the polynomial fit
ordercoeff = 1;

%REQUIRES ATTENTION
xstart = 19; %start and end values to cover the range
%REQUIRES ATTENTION
xend = 26; %of the measured data
%values to plot the best fit curve over
x0 = linspace(xstart,xend,100); 




%copying variable names for copy pasta purposes
xmeas1 = distfromshore(start:ending);
ymeas1 = temp_filtered(start:ending);

%coefficients of the fit and information about the errors
[coefficients1, S1] = polyfit(xmeas1, ymeas1, orderforce); 

%bestfit is the y values of the best fit, delta is something about the errors
[bestfit1, delta1] = polyval(coefficients1, x0, S1);

%uncertainties on the best fit line
upper1 = bestfit1 + delta1;
lower1 = bestfit1 - delta1;




%copying variable names for copy pasta purposes
xmeas2 = distfromshore(start:ending);
ymeas2 = turb_filtered(start:ending);

%coefficients of the fit and information about the errors
[coefficients2, S2] = polyfit(xmeas2, ymeas2, orderforce); 

%bestfit is the y values of the best fit, delta is something about the errors
[bestfit2, delta2] = polyval(coefficients2, x0, S2);

%uncertainties on the best fit line
upper2 = bestfit2 + delta2;
lower2 = bestfit2 - delta2;


%copying variable names for copy pasta purposes
xmeas3 = distfromshore(start:ending);
ymeas3 = straight_filtered(start:ending);

%coefficients of the fit and information about the errors
[coefficients3, S3] = polyfit(xmeas3, ymeas3, orderforce); 

%bestfit is the y values of the best fit, delta is something about the errors
[bestfit3, delta3] = polyval(coefficients3, x0, S3);

%uncertainties on the best fit line
upper3 = bestfit3 + delta3;
lower3 = bestfit3 - delta3;


%copying variable names for copy pasta purposes
xmeas4 = distfromshore(start:ending);
ymeas4 = turbidity(start:ending);

%coefficients of the fit and information about the errors
[coefficients4, S4] = polyfit(xmeas4, ymeas4, ordercoeff); 

%bestfit is the y values of the best fit, delta is something about the errors
[bestfit4, delta4] = polyval(coefficients4, x0, S4);

%uncertainties on the best fit line
upper4 = bestfit4 + delta4;
lower4 = bestfit4 - delta4;




figure (8)
plot(xmeas1, ymeas1,'bo');
hold on;
plot(x0, bestfit1);
hold on;
plot(x0, upper1, '-r');
hold on;
plot(x0, lower1, '-r');
title('temp vs distance');

figure (9)
plot(xmeas2, ymeas2,'bo');
hold on;
plot(x0, bestfit2);
hold on;
plot(x0, upper2, '-r');
hold on;
plot(x0, lower2, '-r');
title('90 deg vs distance');

figure (10)
plot(xmeas3, ymeas3,'bo');
hold on;
plot(x0, bestfit3);
hold on;
plot(x0, upper3, '-r');
hold on;
plot(x0, lower3, '-r');
title('straight vs distance');

figure (11)
plot(xmeas4, ymeas4,'bo');
hold on;
plot(x0, bestfit4);
hold on;
plot(x0, upper4, '-r');
hold on;
plot(x0, lower4, '-r');
title('turbidity vs distance');
% %% Section 3
% %812 pixels = 100 m
% x = x(abs(x)<1000);
% y = y(abs(y)<1000);
% HMC = imread('HMC.png');
% imshow(HMC);
% hold on;
% x_ideal = linspace(125,150);
% y_ideal = linspace(-40,-40);
% plot(8.12*x_ideal+46,-8.12*y_ideal+54,'or',8.12*x+46,-8.12*y+54,'ob');
% hold off;
% heading = heading(1115:2841);
% heading = heading+(pi);
% for i = 1:length(heading)
%     if (heading(i)>3)
%         heading(i) = heading(i)-(pi);
%     end
% end
% heading = heading-(pi/2);
% timeVector = 0:length(heading)-1;
% timeVector = timeVector./10;
% plot(timeVector,heading);
% hold on;
% plot(timeVector,heading*1.5);
% xlabel('Time (s)');
% ylabel('Data');
% legend('Angle Error (radians)','Control Effort u');
% title('Angle Error and Control Effort vs. Time');
