/*
 * File: comp_filt_step.h
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 15-Jun-2022 11:53:36
 */

#ifndef COMP_FILT_STEP_H
#define COMP_FILT_STEP_H

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "comp_filt_init_types.h"

/* Function Declarations */
extern void comp_filt_step(const struct0_T *s, float acc_b[3], float mag_b[3],
  const float gyr_b[3], float qib_est[4]);

#endif

/*
 * File trailer for comp_filt_step.h
 *
 * [EOF]
 */
