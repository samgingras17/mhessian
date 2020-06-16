#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mex.h"

void ErrMsgTxt(bool assertion, const char *text)
{
    if(!assertion)
        mexErrMsgTxt(text);
}

void mxCheckStruct(const mxArray *prhs)
{
    ErrMsgTxt( mxIsStruct(prhs),
        "Invalid input argument: structure argument expected");
}

void mxCheckVector(const mxArray *prhs)
{
    ErrMsgTxt( mxIsDouble(prhs),
        "Invalid input argument: vector of double expected");
    ErrMsgTxt( mxGetN(prhs),
        "Invalid input argument: column vector expected");
}

// void mxCheckScalar(const mxArray *prhs)
// {
//
// }

void mxCheckVectorSize(int n, const mxArray *prhs)
{
    ErrMsgTxt( mxGetM(prhs) == n,
        "Invalid input argument: incompatible vector length");
}

// void mxCheckVectorCompatibility(const mxArray *prhs1, const mxArray *prhs2)
// {
//
// }