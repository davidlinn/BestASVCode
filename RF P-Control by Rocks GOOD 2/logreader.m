
clear;
clf;

filenum = '004';
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
figure;
x = x(1:end);
y = y(1:end);
x = x(abs(x)<1000);
y = y(abs(y)<1000);
%plot(x,y);
for i = 1:length(x)
     plot(x(1:i),y(1:i),'bo');
     pause(0.001);
end


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
