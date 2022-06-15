% CODEGEN_SCRIPT   Generate static library comp_filt_init from comp_filt_init,
%  comp_filt_step.
% 
% Script generated from project 'main.prj' on 15-Jun-2022.
% 
% See also CODER, CODER.CONFIG, CODER.TYPEOF, CODEGEN.

%% Create configuration object of class 'coder.EmbeddedCodeConfig'.
cfg = coder.config('lib','ecoder',true);
cfg.GenerateCodeMetricsReport = true;
cfg.HighlightPotentialDataTypeIssues = true;
cfg.GenerateReport = true;
cfg.EnableVariableSizing = false;
cfg.MATLABSourceComments = true;
cfg.GenCodeOnly = true;
cfg.TargetLangStandard = 'C99 (ISO)';
cfg.HardwareImplementation.ProdHWDeviceType = 'ARM Compatible->ARM Cortex';
cfg.HardwareImplementation.TargetHWDeviceType = 'ARM Compatible->ARM Cortex';

%% Define argument types for entry-point 'comp_filt_init'.
ARGS = cell(2,1);
ARGS{1} = cell(7,1);
ARGS{1}{1} = coder.typeof(single(0),[3 1]);
ARGS{1}{2} = coder.typeof(single(0),[3 1]);
ARGS{1}{3} = coder.typeof(single(0));
ARGS{1}{4} = coder.typeof(single(0));
ARGS{1}{5} = coder.typeof(single(0));
ARGS{1}{6} = coder.typeof(single(0));
ARGS{1}{7} = coder.typeof(single(0));

%% Define argument types for entry-point 'comp_filt_step'.
ARGS{2} = cell(4,1);
ARGS{2}{1} = struct;
ARGS{2}{1}.acc_i = coder.typeof(single(0),[3 1]);
ARGS{2}{1}.mag_i = coder.typeof(single(0),[3 1]);
ARGS{2}{1}.fs = coder.typeof(single(0));
ARGS{2}{1}.K_cf = coder.typeof(single(0));
ARGS{2}{1}.dJ_max = coder.typeof(single(0));
ARGS{2}{1}.N_max = coder.typeof(single(0));
ARGS{2}{1}.alpha = coder.typeof(single(0));
ARGS{2}{1}.dt = coder.typeof(single(0));
ARGS{2}{1}.qib_prev = coder.typeof(single(0),[4 1]);
ARGS{2}{1} = coder.typeof(ARGS{2}{1});
ARGS{2}{2} = coder.typeof(single(0),[3 1]);
ARGS{2}{3} = coder.typeof(single(0),[3 1]);
ARGS{2}{4} = coder.typeof(single(0),[3 1]);

%% Invoke MATLAB Coder.
codegen -config cfg -singleC comp_filt_init -args ARGS{1} comp_filt_step -args ARGS{2}
