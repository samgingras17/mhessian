#include "mex.h"

#ifndef MEX_GRAD_HESS
#define MEX_GRAD_HESS

typedef struct {
    double Q_11;
    double Q_tt;
    double Q_ttp;
    double q_1;
    double q_2;
    double m_tm1[3];
    double m_t[3];
    double v_tm1[3];
    double v_t[3];
} Q_term;

void compute_grad_Hess(
        const mxArray *mxState,
        int n,
        double *mu,
        double phi,
        double omega,
        double *u,
        double *grad,
        double *Hess,
        double *var
    );
#endif