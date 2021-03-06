% New approximations of Egrad, EHess and Vgrad
% New way of setting H(1, 1)
% New way of (not) setting H(2, 3)
function [g, H, V, Hfull] = grad_hess_approx1(theta, state, long_th)
    [g, Hfull, V] = new_grad_hess_approx(theta, state, long_th);
    H = Hfull;
    if( isfield(theta, 'x') )
        n = theta.x.N;
    else
        n = theta.N;
    end
    H(1, 1) = -n/2;
    H(1, 2) = 0; H(2, 1) = 0;
    if length(g) > 2
        H(1, 3) = 0; H(3, 1) = 0;
    end
end