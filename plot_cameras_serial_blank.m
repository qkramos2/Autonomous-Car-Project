% Real time data collection example
%
% This script is implemented as a function so that it can
%   include sub-functions
%
% This script can be modified to be used on any platform by changing the
% serialPort variable.
% Example:-
% On Linux:     serialPort = '/dev/ttyS0';
% On MacOS:     serialPort = '/dev/tty.KeySerial1';
% On Windows:   serialPort = 'COM1';
%
%To run: 
%plot_cams()
%To reset ports: (if MATLAB still thinks they're busy)
%delete(instrfindall)

function plot_cameras_serial_blank 

    %Configure serial connection
    serialPort = 'COM4';
    serialObject = serial(serialPort);
    serialObject.BaudRate = 9600;

    %Initiate serial connection
    fopen(serialObject);

    % This gets called on cleanup to ensure the stream gets closed
    finishup = onCleanup(@() myCleanupFun(serialObject));

    % Instantiate variables
    count = 1;
    trace = zeros(1, 128);          %Stored Values for Raw Input
    bintrace = zeros(1,128);        %Stored Values for Edge Detection
    smoothtrace  = zeros(1,128);    %Stored Values for 5-Point Averager
    global maxval
    maxval = 0;

    while (1)
        % Check for data in the stream
        if serialObject.BytesAvailable
            val = fscanf(serialObject,'%i');
            if ((val == -1) || (val == -3))
                count = 1;
            elseif (val == -2)
                if (count >= 128)
                    plotdata(trace, 1);
                end
                count = 1;
            elseif (val == -4)
                count = 1;
                plotdata(trace, 2);
            else
                trace(count) = val;
                count = count + 1;
            end
        end
    end

    % Clean up the serial object
    fclose(serialObject);
    delete(serialObject);
    clear serialObject;

end %plot_cams

%**************************************************************************
%**************************************************************************

function plotdata(trace, cam)
    % RAW PLOT
    drawnow;
    subplot(4,2,cam);
    plot(trace);
    
    % 5-Point Average Plot
    smoothtrace = smooth(trace, 5);
    subplot(4,2,cam+2);
    plot(smoothtrace);
    
    % Binary Trace
    % Calculate 1's and 0's via thresholding
    global maxval;
    
    if (maxval < max(smoothtrace))
        maxval = max(smoothtrace);
    end
    
    for i = 1:128
        %Edge detection (binary 0 or 1)
        if (smoothtrace(i) > maxval/2)
            bintrace(i) = 1;
        else
            bintrace(i) = 0;
        end
    end
    
    drawnow;
    subplot(4,2,cam+4);
    plot(bintrace);

end

%**************************************************************************
%**************************************************************************

function myCleanupFun(serialObject)
    % Clean up the serial object
    fclose(serialObject);
    delete(serialObject);
    clear serialObject;
    delete(instrfindall);
end

%**************************************************************************
%**************************************************************************
