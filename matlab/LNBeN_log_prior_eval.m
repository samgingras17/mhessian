function [lnp_th, lnp_th_g, lnp_th_H] = LNBeN_log_prior_eval(hyper, theta)

	[v1, g1, H11] = LN_sigma(exp(-0.5*theta.th(1)), hyper.LN_mu, hyper.LN_h);
	[v2, g2, H22] = Be_phi(tanh(theta.th(2)), hyper.Be_al, hyper.Be_be);
	if hyper.has_mu
		u = theta.th(3) - hyper.N_mu;
		g3 = -hyper.N_h * u;
		H33 = -hyper.N_h;
		lnp_th = v1 + v2 - 0.5 * hyper.N_h * u^2;
		lnp_th_g = [g1; g2; g3];
		lnp_th_H = diag([H11; H22; H33]);
	else
		lnp_th = v1 + v2;
		lnp_th_g = [g1; g2];
		lnp_th_H = diag([H11; H22]);
	end
end

function [lnf, dlnf_dth, d2lnf_dth2] = LN_sigma(sigma, mu, h)

	% Value of log Log-Normal(mu, h^-1) density on (0,infty), and two derivatives, at sigma
	lnf_sigma = -log(sigma) - 0.5*h*(log(sigma) - mu)^2;
	dlnf_dsigma = sigma^(-1) * (1 + h*(log(sigma) - mu));
	d2lnf_dsigma = sigma^(-2) * (1 + h*(log(sigma) - mu - 1));

	lnf = lnf_sigma + log(sigma);
	dlnf_dth = -0.5 * (dlnf_dsigma*sigma + 1);
	d2lnf_dth2 = 0.25*sigma * (d2lnf_dsigma*sigma + dlnf_dsigma);  
end

function [lnf, dlnf_dth, d2lnf_dth2] = Be_phi(phi, alpha, beta)

	% Value of log beta(alpha, beta) density on (-1,1), and two derivatives, at phi
	lnf_phi = (alpha-1)*log(1+phi) + (beta-1)*log(1-phi);
	dlnf_dphi = (alpha-1)./(1+phi) - (beta-1)./(1-phi);
	d2lnf_dphi2 = -(alpha-1)./(1+phi).^2 - (beta-1)./(1-phi).^2;
	
	lnf = lnf_phi + log(1-phi.^2);
	dlnf_dth = dlnf_dphi .* (1-phi.^2) - 2*phi;
	d2lnf_dth2 = d2lnf_dphi2 .* (1-phi.^2).^2 - dlnf_dphi .* (2*phi).*(1-phi.^2) - 2*(1-phi.^2);
end
