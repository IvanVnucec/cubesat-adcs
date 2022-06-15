/*
 * File: comp_filt_init.h
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 15-Jun-2022 15:26:49
 */

#ifndef COMP_FILT_INIT_H
#define COMP_FILT_INIT_H

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "comp_filt_init_types.h"

/* Function Declarations */
extern void comp_filt_init(const float acc_i[3], const float mag_i[3], float fs,
  float K_cf, float dJ_max, float N_max, float alpha, struct0_T *s);

#endif

/*
 * File trailer for comp_filt_init.h
 *
 * [EOF]
 */
