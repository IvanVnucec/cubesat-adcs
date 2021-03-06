function [fitresult, gof] = fit_step_response(t, wz_clean)
%CREATEFIT(T,WZ_CLEAN)
%  Create a fit.
%
%  Data for 'fit_step_response' fit:
%      X Input : t
%      Y Output: wz_clean
%  Output:
%      fitresult : a fit object representing the fit.
%      gof : structure with goodness-of fit info.
%
%  See also FIT, CFIT, SFIT.

%  Auto-generated by MATLAB on 14-Jun-2022 16:42:14


%% Fit: 'fit_step_response'.
[xData, yData] = prepareCurveData( t, wz_clean );

% Set up fittype and options.
ft = fittype( 'K*(1 - exp(-t./Tau))', 'independent', 't', 'dependent', 'y' );
opts = fitoptions( 'Method', 'NonlinearLeastSquares' );
opts.Display = 'Off';
opts.StartPoint = [0.0975404049994095 0.546881519204984];

% Fit model to data.
[fitresult, gof] = fit( xData, yData, ft, opts );

