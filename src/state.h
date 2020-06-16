#include "mex.h"

#ifndef MEX_STATE
#define MEX_STATE

typedef struct {
    int n;              // Nb of observation
    int m;              // Nb of state
    double *y;          // Vector of observations
    
    // SG: NEW variables for regime duration model
    double *z;      // Latent indicator for regular/burst regime
    double *k;      // State indicator
    int *p;         // Position of state (computed using state indicator)
} Data;

typedef struct {
    int n;              // Nb of scalar
    int m;              // Nb of mixture
    double *scalar;

    double eta;
    double kappa;
    double lambda;
} Parameter;

typedef struct {
    int n;
    int is_basic;
    
    double alpha_mean;
    double phi;
    double omega;
    
    double *d_tm;
    double *mu_tm;
    double *phi_tm;
    double *omega_tm;
} State_parameter;

typedef struct {
    Parameter *y;
    State_parameter *alpha;
} Theta;

typedef struct {
    int n;
    int sign;
    int psi_stride;
    
    // Computation options
    double tolerance;
    int max_iterations;                 // WJM: delete if not using
    int max_iterations_safe;
    int max_iterations_unsafe;
    int n_alpha_partials;
    
    int circ_buffer_pos;                // SG: not used
    int circ_buffer_len;                // SG: not used
    double *inf_norm_circ_buffer;       // SG: not used
    
    
    int iteration;                      // SG: not used
    int guess_alC;                      // SG: for initialization
    int trust_alC;                      // SG: remove as input for compute_alC (use state)
    int safe;                           // SG: remove as input for compute_alC (use state)
    
    // Computation variables
    double *alpha;
    double *Hb_0;
    double *Hb_1;
    double *cb;
    double *Hbb_0;
    double *Hbb_1;
    double *Hbb_1_2;
    double *cbb;
    double *alC;
    double *Sigma_prior;
    double *ad_prior;
    double *m_prior;
    double *Sigma;
    double *m;
    double *ad;
    double *add;
    double *addd;
    double *adddd;
    double *b;
    double *bd;
    double *bdd;
    double *bddd;
    double *mu;
    double *mud;
    double *mudd;
    double *s;
    double *sd;
    double *sdd;
    double *sddd;
    double *eps;
    double *a;
    double *psi;
    
    // Diagnostic variables
    // ...
} State;

typedef struct {
    void (*initialize)(void);
    
    int n_partials_t;
    int n_partials_tp1;
    
    char *usage_string;
    
    void (*initializeParameter)(const mxArray *prhs, Parameter *theta_y);
    void (*read_data)(const mxArray *prhs, Data *data);
    
    void (*draw_y__theta_alpha)(double *alpha, Parameter *theta_y, Data *data);
    void (*log_f_y__theta_alpha)(double *alpha, Parameter *theta_y, Data *data, double *log_f);
    
    void (*compute_derivatives_t)(Theta *theta, Data *data, int t, double alpha, double *psi_t);
    void (*compute_derivatives)(Theta *theta, State *state, Data *data);
} Observation_model;

typedef struct {
    char *Matlab_field_name;
    double **C_field_pointer;
} Field;

void initializeStateParameter( const mxArray *prhs, State_parameter *theta_alpha );
void setDefaultOptions( State *state );
void readComputationOptions( const mxArray *prhs, State *state );
mxArray *mxStateAlloc( int n, Observation_model *model, State *state );
Theta *mxThetaAlloc( void );
#endif