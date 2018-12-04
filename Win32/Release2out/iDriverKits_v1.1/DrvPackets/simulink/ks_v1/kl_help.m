function kl_help(varargin)

% get current block
blockName = '';

if nargin ~= 0
    aliasTag = varargin{1};
    %maskType = get_param(blockName, 'MaskType');
    switch(lower(aliasTag))
        % Set the MaskDisplay parameter of the icon Mask ;
		% eval('kl_help(''init'');');
        case 'init'
            blockName = 'KL_Ethernet_init.htm';
        case 'send'
            blockName = 'KL_Ethernet_send.htm';
		case 'receive'
            blockName = 'KL_Ethernet_receive.htm';
        otherwise
            blockName = '';
    end
end   

% If no block is selected, display home page
if isempty(blockName)
    blockName = 'Reference';
end

curpath = pwd;
new_curpath = strrep(curpath, '\', '/');
chmname='/help/KL_Ethernet.chm::/';

% Specify a page
path=[new_curpath chmname blockName];
h='hh';
command = [h ' ' path];
system([command ' &']);



