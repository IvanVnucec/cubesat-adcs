/*
 * File: _coder_comp_filt_init_api.h
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 15-Jun-2022 15:26:49
 */

#ifndef _CODER_COMP_FILT_INIT_API_H
#define _CODER_COMP_FILT_INIT_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_comp_filt_init_api.h"

/* Type Definitions */
#ifndef typedef_struct0_T
#define typedef_struct0_T

typedef struct {
  real32_T acc_i[3];
  real32_T mag_i[3];
  real32_T fs;
  real32_T K_cf;
  real32_T dJ_max;
  real32_T N_max;
  real32_T alpha;
  real32_T dt;
  real32_T qib_prev[4];
} struct0_T;

#endif                                 /*typedef_struct0_T*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void comp_filt_init(real32_T acc_i[3], real32_T mag_i[3], real32_T fs,
  real32_T K_cf, real32_T dJ_max, real32_T N_max, real32_T alpha, struct0_T *s);
extern void comp_filt_init_api(const mxArray * const prhs[7], const mxArray
  *plhs[1]);
extern void comp_filt_init_atexit(void);
extern void comp_filt_init_initialize(void);
extern void comp_filt_init_terminate(void);
extern void comp_filt_init_xil_terminate(void);
extern void comp_filt_step(struct0_T *s, real32_T acc_b[3], real32_T mag_b[3],
  real32_T gyr_b[3], real32_T qib_est[4]);
extern void comp_filt_step_api(const mxArray * const prhs[4], const mxArray
  *plhs[2]);

#endif

/*
 * File trailer for _coder_comp_filt_init_api.h
 *
 * [EOF]
 */
