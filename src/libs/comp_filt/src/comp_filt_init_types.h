/*
 * File: comp_filt_init_types.h
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 15-Jun-2022 15:26:49
 */

#ifndef COMP_FILT_INIT_TYPES_H
#define COMP_FILT_INIT_TYPES_H

/* Include Files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef typedef_struct0_T
#define typedef_struct0_T

typedef struct {
  float acc_i[3];
  float mag_i[3];
  float fs;
  float K_cf;
  float dJ_max;
  float N_max;
  float alpha;
  float dt;
  float qib_prev[4];
} struct0_T;

#endif                                 /*typedef_struct0_T*/
#endif

/*
 * File trailer for comp_filt_init_types.h
 *
 * [EOF]
 */
