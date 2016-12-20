function rec = readlog2(filename)
%READLOG Read QRoboticsCenter log file and determine the right class

if ~exist(filename,'file')
    error('File does not exist or is not on the matlab path');
end

DOM = xmlread(filename);
type = char(DOM.getDocumentElement().getTagName());

switch type
    case 'QGPIORecord'
        rec = QRecord(filename);
    case 'QAttitudeRecord'
        error('Not yet implemented');
    case 'QPositionRecord'
        rec = QPositionRecord(filename);       
end
end

