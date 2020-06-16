#include <math.h>
#include <string.h>
#include "mex.h"
#include "RNG.h"
#include "state.h"
#include "errors.h"


static int n_theta = 0;
static int n_partials_t = 5;
static int n_partials_tp1 = 0;


static char *usage_string = 
"Name: plain_SV\n"
"Description: Univariate Gaussian stochastic volatility model, without leverage\n"
"Extra parameters: none\n";

static void initializeParameter(const mxArray *prhs, Parameter *theta_y)
{
    theta_y->n = n_theta;
}

static void read_data(const mxArray *prhs, Data *data)
{
    mxArray *pr_y = mxGetField(prhs,0,"y");
    
    ErrMsgTxt( pr_y != NULL,
    "Invalid input argument: data struct: 'y' field missing");
    ErrMsgTxt( mxGetN(pr_y),
    "Invalid input argument: data struct: column vector expected");
    
    data->n = mxGetM(pr_y);
    data->m = mxGetM(pr_y);
    data->y = mxGetPr(pr_y);
}

static void draw_y__theta_alpha(double *alpha, Parameter *theta_y, Data *data)
{
    int t, n = data->n;
    for (t=0; t<n; t++)
        data->y[t] = exp(alpha[t]/2) * rng_gaussian();
}

static void log_f_y__theta_alpha(double *alpha, Parameter *theta_y, Data *data, double *log_f)
{
    int t, n = data->n;
    double result = 0.0;
    for(t=0; t<n; t++)
    {
        double y_t_2 = data->y[t] * data->y[t];
        result -= y_t_2 * exp(-alpha[t]) + alpha[t];
    }
    result -= n * log(2 * M_PI);
    *log_f = 0.5 * result;
}

static inline void derivative(double y_t, double alpha_t, double *psi_t)
{
    psi_t[4] = psi_t[2] = -0.5 * (y_t * y_t) * exp( -alpha_t );
    psi_t[1] = -psi_t[2] - 0.5;
    psi_t[5] = psi_t[3] = -psi_t[2];
}

static void compute_derivatives_t(Theta *theta, Data *data, int t, double alpha, double *psi_t) 
{
    derivative(data->y[t], alpha, psi_t);
}

static void compute_derivatives( Theta *theta, State *state, Data *data )
{
    int t, n = state->n;
    double *alpha = state->alC; 
    double *psi_t;
    for(t = 0, psi_t = state->psi; t < n; t++, psi_t += state->psi_stride)
        derivative(data->y[t], alpha[t], psi_t);
}

static void initialize(void);

Observation_model plain_SV = { initialize, 0 };

static void initialize()
{
    plain_SV.n_partials_t = n_partials_t;
    plain_SV.n_partials_tp1 = n_partials_tp1;
    
    plain_SV.usage_string = usage_string;
    
    plain_SV.initializeParameter = initializeParameter;
    plain_SV.read_data = read_data;
    
    plain_SV.draw_y__theta_alpha = draw_y__theta_alpha;
    plain_SV.log_f_y__theta_alpha = log_f_y__theta_alpha;
    
    plain_SV.compute_derivatives_t = compute_derivatives_t;
    plain_SV.compute_derivatives = compute_derivatives;
}