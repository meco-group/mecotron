classdef QRecord
    %QRECORD Recordings of QRoboticsCenter
    %   Stores recordings of QRoboticsCenter
    %   Has fields:
    %       TYPE
    %       TIME
    %       VERSION
    %       COMMENT
    %       LABELS
    %       DATA
    
    properties
        file
        type
        time
        version
        comment
        labels
        data
    end
    
    methods
        function self = QRecord(filename)
            if ~exist(filename,'file')
                error('File does not exist or is not on the matlab path');
            end
            
            DOM = xmlread(filename);
            self.file = filename;
            self.type = char(DOM.getDocumentElement().getTagName());
            
            % Get the time
            item_time = DOM.getElementsByTagName('time').item(0);
            year = str2double(char(item_time.getElementsByTagName('year').item(0).getFirstChild.getData()));
            month = str2double(char(item_time.getElementsByTagName('month').item(0).getFirstChild.getData()));
            day = str2double(char(item_time.getElementsByTagName('day').item(0).getFirstChild.getData()));
            hour = str2double(char(item_time.getElementsByTagName('hour').item(0).getFirstChild.getData()));
            minute = str2double(char(item_time.getElementsByTagName('minute').item(0).getFirstChild.getData()));
            self.time = struct('year',year,'month',month,'day',day,'hour',hour,'minute',minute);
            
            % Get the version
            self.version = char(DOM.getElementsByTagName('version').item(0).getFirstChild.getData);
           
            % Get the comments
            self.comment = char(DOM.getElementsByTagName('comment').item(0).getFirstChild.getData);
            
            % Get the labels
            item_labels = DOM.getElementsByTagName('labels').item(0);
            element_values = item_labels.getElementsByTagName('value');
            self.labels = cell(1,element_values.getLength());
            for k = 1:element_values.getLength()
                self.labels{k} = char(element_values.item(k-1).getFirstChild.getData());
            end
            
            % Get the data
            item_data = DOM.getElementsByTagName('data').item(0);
            element_rows = item_data.getElementsByTagName('row');
            
            n1 = element_rows.getLength();
            n2 = length(strsplit(char(element_rows.item(0).getFirstChild.getData),'\t'));
            self.data = zeros(n1,n2); % allocate data array
            
            for k = 1:n1
                row = char(element_rows.item(k-1).getFirstChild.getData);
                self.data(k,:) = cellfun(@(x)str2double(x),strsplit(row,'\t'));
            end
            
            if n2 ~= length(self.labels)
                warning('Length of the labels does not match the data size');
            end
        end
        
        function data = getData(self,varargin)
            if nargin == 1
                data = self.data;
            else
                labels_ = varargin{1};
                if iscell(labels_)
                    data = zeros(size(self.data,1),length(labels_));
                    for k = 1:length(labels_)
                        data(:,k) = getData(self,labels_{k});
                    end
                else
                    match = strcmp(self.labels,labels_);
                    if ~any(match)
                        error(['Could not find a match for ''' labels_ '''']);
                    end
                    data = self.data(:,match);
                end
            end
        end
        
        function disp(self)
            fprintf(['\t',self.type, ', recorded on ', num2str(self.time.day),'-',num2str(self.time.month),'-',num2str(self.time.year),'\n']);
            fprintf('\tcomments:\n');
            fprintf(['\t    ',self.comment,'\n']);
        end
    end
    
end

