classdef QPositionRecord < QRecord
    %QPOSITIONRECORD Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        excitation
        FRF
    end
    
    methods
        function self = QPositionRecord(filename)
            self@QRecord(filename);
            DOM = xmlread(filename);            
            
            % Get the excitation
            item_time = DOM.getElementsByTagName('excitation').item(0);
            type = str2double(char(item_time.getElementsByTagName('type').item(0).getFirstChild.getData()));
            fmin = str2double(char(item_time.getElementsByTagName('fmin').item(0).getFirstChild.getData()));
            fmax = str2double(char(item_time.getElementsByTagName('fmax').item(0).getFirstChild.getData()));
            period = str2double(char(item_time.getElementsByTagName('period').item(0).getFirstChild.getData()));
            self.excitation = struct('type',type,'fmin',fmin,'fmax',fmax,'period',period);
        
            % Compute the frf
            self = computeFRF(self);
        end
        
        function self = computeFRF(self)
            
        end
        
        function FRF = getFRF(self)
            FRF = self.FRF;
        end
    end
    
end

