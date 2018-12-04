function blkStruct = slblocks
blkStruct.Name = ['KeLiang I/O'];
blkStruct.OpenFcn = 'kl_io';
blkStruct.MaskDisplay = '';

Browser(1).Library = 'kl_io';
Browser(1).Name    = 'KeLiang I/O';
Browser(1).IsFlat  = 0;% Is this library "flat" (i.e. no subsystems)?

% Browser(2).Library = 'kl_dl';
% Browser(2).Name    = 'KeLiang PowerSys';
% Browser(2).IsFlat  = 0;% Is this library "flat" (i.e. no subsystems)?

% Browser(3).Library = 'kl_qc';
% Browser(3).Name    = 'KeLiang Automotive';
% Browser(3).IsFlat  = 0;% Is this library "flat" (i.e. no subsystems)?

% Browser(4).Library = 'kl_jg';
% Browser(4).Name    = 'KeLiang Aerospace';
% Browser(4).IsFlat  = 0;% Is this library "flat" (i.e. no subsystems)?

blkStruct.Browser = Browser;

% End of slblocks

%
%  KL Technologies inc
%
%  Copyright (C) 2018. All rights reserved
%  Internal m files

