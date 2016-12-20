clear all
close all
clc

% Read the file and save the recording
filename = 'log_gpio_10_25_10_18';
rec = readlog([filename '.xml']); % use readlog to automatically decide on the class
save(filename,'rec'); % Reading the xml takes some time, so it might be better to save it to a mat file so that acces is quicker

% Extract some signals from the data
time = rec.getData('time');
error1 = rec.getData('Error1');
actuator1 = rec.getData('Actuator1');
error2 = rec.getData('Error2');
actuator2 = rec.getData('Actuator2');

% Do some plotting
figure, subplot(221), plot(time,error1), title('error1')
subplot(222), plot(time,actuator1), title('actuator1')
subplot(223), plot(time,error2), title('error2')
subplot(224), plot(time,actuator2), title('actuator2')

% Access some fields from the recording
rec.type        % what type is the recording
rec.comment     % what comment did you add to the file (check the comment field in the xml file)
rec.labels      % what are the signal labels
rec.time        % when was the file recorded

% Displaying the recording gives some info:
rec